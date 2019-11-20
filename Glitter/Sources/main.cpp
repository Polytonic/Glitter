// Local Headers
#include "glitter.hpp"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>

#include "box_textures.hpp"
#include "elementary_models.hpp"
#include "iterable_mesh.hpp"
#include "learnopengl/camera.h"
#include "learnopengl/model.h"
#include "learnopengl/shader_m.h"
#include "mesh_iterator.hpp"
#include "point_shadows_rt_renderer.hpp"
#include "rt_renderer.hpp"
#include "simple_rt_renderer.hpp"
#include "texture_gen.hpp"

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  std::default_random_engine random_gen(4);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  std::unique_ptr<RtRenderer> renderer(new PointShadowsRtRenderer());
  // std::unique_ptr<RtRenderer> renderer(new SimpleRtRenderer());
  GLFWwindow* window = renderer->OpenWindow("RT Render");

  /*{
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "nanosuit";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir)
        .append(model_dir)
        .append("/")
        .append(object_file)
        .append(file_type);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.0f, 0.0f));
    model_mat = glm::scale(
        model_mat, glm::vec3(0.1f, 0.1f, 0.1f));  // scale nanosuit model
    renderer->AddModel(file_path, model_mat);
  }
  {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "sponza";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir)
        .append(model_dir)
        .append("/")
        .append(object_file)
        .append(file_type);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.0f, 0.0f));
    model_mat = glm::scale(
        model_mat, glm::vec3(0.005f, 0.005f, 0.005f));  // scale sponza model
    renderer->AddModel(file_path, model_mat);
  }*/
  {
    Texture texture = GetWhiteTexture();
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(1.0f, 5, 0.3f, 0.25f));
    BasicMeshIterator mesh_iterator(20, 1000);
    mesh_iterator.SetIterableMesh(std::move(it_mesh), true);
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::scale(model_mat, glm::vec3(0.5f, 0.5f, 0.5f));
    renderer->AddModel(std::move(generated_model), model_mat);
  }
  {
    Texture texture = GetTestBoxTexture(&random_gen);
    std::unique_ptr<IterableMesh> it_mesh(
        new IterableHelix(1.0f, 5, 0.3f, 0.25f));
    BasicMeshIterator mesh_iterator(20, 1000);
    mesh_iterator.SetIterableMesh(std::move(it_mesh), true);
    MeshVertices mesh_vert = mesh_iterator.GetMesh();
    Mesh mesh(mesh_vert.vertices, mesh_vert.indices, {texture});
    std::unique_ptr<Model> generated_model(new Model({mesh}));
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::scale(model_mat, glm::vec3(0.5f, 0.5f, 0.5f));
    model_mat = glm::rotate(model_mat, (float)M_PI, glm::vec3(0, 1.0f, 0));
    renderer->AddModel(std::move(generated_model), model_mat);
  }

  // render loop
  // -----------
  std::cerr << "Starting rendering" << std::endl;
  while (!glfwWindowShouldClose(window)) {
    renderer->Render();
  }

  glfwTerminate();
  return 0;
}
