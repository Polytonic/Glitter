#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>  // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "learnopengl/glitter.hpp"
#include "learnopengl/shader.h"
#include "scene/primitives.hpp"
#include "shapes/renderable.hpp"
#include "tracer/intersectable.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Model;

class Mesh : public Renderable {
 public:
  /*  Mesh Data  */
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  Material material_;
  unsigned int VAO;

  /*  Functions  */
  // constructor
  Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material,
       glm::mat4 local_model_mat = glm::mat4(1.0f))
      : material_(material) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->local_model_mat_ = local_model_mat;

    // now that we have all the required data, set the vertex buffers and its
    // attribute pointers.
    setupMesh();
  }

  // render the mesh
  void Draw(ShaderSet shaders, glm::mat4 model_mat) override {
    // bind textures
    glActiveTexture(GL_TEXTURE0);
    // retrieve texture number (the N in diffuse_textureN)
    string number = "0";
    string name = material_.diff_texture().type;

    // now set the sampler to the correct texture unit
    glUniform1i(glGetUniformLocation(shaders.texture_shader->ID,
                                     (name + number).c_str()),
                0);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, material_.diff_texture().id);

    shaders.texture_shader->setMat4("model", model_mat * local_model_mat_);
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
  }

  void GetTris(glm::mat4 model_mat, std::vector<InterPtr>* tris) override;

  glm::mat4 local_model_mat() const { return local_model_mat_; }

  void set_parent(Model* parent) { parent_ = parent; }
  Model* parent() { return parent_; }

 private:
  /*  Render data  */
  unsigned int VBO, EBO;
  glm::mat4 local_model_mat_;

  Model* parent_;

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
