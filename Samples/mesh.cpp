// Preprocessor Directives
#define STB_IMAGE_IMPLEMENTATION

// Local Headers
#include "mesh.hpp"

// System Headers
#include <stb_image.h>

// Define Namespace
namespace Mirage
{
    Mesh::Mesh(std::string const & filename) : Mesh()
    {
        // Load a Model from File
        Assimp::Importer loader;
        aiScene const * scene = loader.ReadFile(
            PROJECT_SOURCE_DIR "/Mirage/Models/" + filename,
            aiProcessPreset_TargetRealtime_MaxQuality |
            aiProcess_OptimizeGraph                   |
            aiProcess_FlipUVs);

        // Walk the Tree of Scene Nodes
        auto index = filename.find_last_of("/");
        if (!scene) fprintf(stderr, "%s\n", loader.GetErrorString());
        else parse(filename.substr(0, index), scene->mRootNode, scene);
    }

    Mesh::Mesh(std::vector<Vertex> const & vertices,
               std::vector<GLuint> const & indices,
               std::map<GLuint, std::string> const & textures)
                    : mIndices(indices)
                    , mVertices(vertices)
                    , mTextures(textures)
    {
        // Bind a Vertex Array Object
        glGenVertexArrays(1, & mVertexArray);
        glBindVertexArray(mVertexArray);

        // Copy Vertex Buffer Data
        glGenBuffers(1, & mVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                     mVertices.size() * sizeof(Vertex),
                   & mVertices.front(), GL_STATIC_DRAW);

        // Copy Index Buffer Data
        glGenBuffers(1, & mElementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     mIndices.size() * sizeof(GLuint),
                   & mIndices.front(), GL_STATIC_DRAW);

        // Set Shader Attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, uv));
        glEnableVertexAttribArray(0); // Vertex Positions
        glEnableVertexAttribArray(1); // Vertex Normals
        glEnableVertexAttribArray(2); // Vertex UVs

        // Cleanup Buffers
        glBindVertexArray(0);
        glDeleteBuffers(1, & mVertexBuffer);
        glDeleteBuffers(1, & mElementBuffer);
    }

    void Mesh::draw(GLuint shader)
    {
        unsigned int unit = 0, diffuse = 0, specular = 0;
        for (auto &i : mSubMeshes) i->draw(shader);
        for (auto &i : mTextures)
        {   // Set Correct Uniform Names Using Texture Type (Omit ID for 0th Texture)
            std::string uniform = i.second;
                 if (i.second == "diffuse")  uniform += (diffuse++  > 0) ? std::to_string(diffuse)  : "";
            else if (i.second == "specular") uniform += (specular++ > 0) ? std::to_string(specular) : "";

            // Bind Correct Textures and Vertex Array Before Drawing
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, i.first);
            glUniform1f(glGetUniformLocation(shader, uniform.c_str()), ++unit);
        }   glBindVertexArray(mVertexArray);
            glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    }

    void Mesh::parse(std::string const & path, aiNode const * node, aiScene const * scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
            parse(path, scene->mMeshes[node->mMeshes[i]], scene);
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            parse(path, node->mChildren[i], scene);
    }

    void Mesh::parse(std::string const & path, aiMesh const * mesh, aiScene const * scene)
    {
        // Create Vertex Data from Mesh Node
        std::vector<Vertex> vertices; Vertex vertex;
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {   if (mesh->mTextureCoords[0])
            vertex.uv       = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal   = glm::vec3(mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z);
            vertices.push_back(vertex);
        }

        // Create Mesh Indices for Indexed Drawing
        std::vector<GLuint> indices;
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);

        // Load Mesh Textures into VRAM
        std::map<GLuint, std::string> textures;
        auto diffuse  = process(path, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
        auto specular = process(path, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR);
        textures.insert(diffuse.begin(), diffuse.end());
        textures.insert(specular.begin(), specular.end());

        // Create New Mesh Node
        mSubMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(vertices, indices, textures)));
    }

    std::map<GLuint, std::string> Mesh::process(std::string const & path,
                                                aiMaterial * material,
                                                aiTextureType type)
    {
        std::map<GLuint, std::string> textures;
        for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
        {
            // Define Some Local Variables
            GLenum format;
            GLuint texture;
            std::string mode;

            // Load the Texture Image from File
            aiString str; material->GetTexture(type, i, & str);
            std::string filename = str.C_Str(); int width, height, channels;
            filename = PROJECT_SOURCE_DIR "/Mirage/Models/" + path + "/" + filename;
            unsigned char * image = stbi_load(filename.c_str(), & width, & height, & channels, 0);
            if (!image) fprintf(stderr, "%s %s\n", "Failed to Load Texture", filename.c_str());

            // Set the Correct Channel Format
            switch (channels)
            {
                case 1 : format = GL_ALPHA;     break;
                case 2 : format = GL_LUMINANCE; break;
                case 3 : format = GL_RGB;       break;
                case 4 : format = GL_RGBA;      break;
            }

            // Bind Texture and Set Filtering Levels
            glGenTextures(1, & texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, format,
                         width, height, 0, format, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Release Image Pointer and Store the Texture
            stbi_image_free(image);
                 if (type == aiTextureType_DIFFUSE)  mode = "diffuse";
            else if (type == aiTextureType_SPECULAR) mode = "specular";
            textures.insert(std::make_pair(texture, mode));
        }   return textures;
    }
};
