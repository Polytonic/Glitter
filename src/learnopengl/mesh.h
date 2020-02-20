#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>  // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glitter.hpp"
#include "learnopengl/shader.h"
#include "renderable.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};

struct DVertex {
  DVertex() = default;
  DVertex(const Vertex& v);
  // position
  DVec3 Position;
  // normal
  DVec3 Normal;
  // texCoords
  DVec2 TexCoords;
  // tangent
  DVec3 Tangent;
  // bitangent
  DVec3 Bitangent;
};

struct Light {
  glm::vec3 Position;
  glm::vec3 Color;

  float Linear = 0.2f;
  float Quadratic = 0.4f;
};

struct Texture {
  unsigned int id = 0;
  string type;
  string path;
  int width = 0;
  int height = 0;
  int num_components = 0;
  unsigned char* data = nullptr;
};

struct Transparency {
  double opacity = 1.0;
  double index = 1.0003;
};

class Material {
 public:
  Material(Texture diff_texture);
  Material(Texture diff_texture, Transparency transparency);

  const Texture& diff_texture() const { return diff_texture_; }
  const Transparency& transparency() const { return transparency_; }
  double opacity() const { return transparency_.opacity; }
  double r_index() const { return transparency_.index; }

 private:
  Texture diff_texture_;
  Transparency transparency_;
};

class Mesh : public Renderable {
 public:
  /*  Mesh Data  */
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Material> materials;
  unsigned int VAO;

  /*  Functions  */
  // constructor
  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Material> materials,
       glm::mat4 local_model_mat = glm::mat4(1.0f)) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->materials = std::move(materials);
    this->local_model_mat_ = local_model_mat;

    // now that we have all the required data, set the vertex buffers and its
    // attribute pointers.
    setupMesh();
  }

  // render the mesh
  void Draw(ShaderSet shaders, glm::mat4 model_mat) override {
    // bind textures
    if (!materials.empty()) {
      glActiveTexture(GL_TEXTURE0);
      // retrieve texture number (the N in diffuse_textureN)
      string number = "0";
      string name = materials[0].diff_texture().type;

      // now set the sampler to the correct texture unit
      glUniform1i(glGetUniformLocation(shaders.texture_shader->ID,
                                       (name + number).c_str()),
                  0);
      // and finally bind the texture
      glBindTexture(GL_TEXTURE_2D, materials[0].diff_texture().id);
    }

    shaders.texture_shader->setMat4("model", model_mat * local_model_mat_);
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
  }

  glm::mat4 local_model_mat() const { return local_model_mat_; }

 private:
  /*  Render data  */
  unsigned int VBO, EBO;
  glm::mat4 local_model_mat_;

  /*  Functions    */
  // initializes all the buffer objects/arrays
  void setupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for
    // all its items. The effect is that we can simply pass a pointer to the
    // struct and it translates perfectly to a glm::vec3/2 array which again
    // translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
  }
};
#endif
