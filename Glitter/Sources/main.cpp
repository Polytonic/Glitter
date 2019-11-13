// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <point_shadows_rt_renderer.hpp>
#include <rt_renderer.hpp>
#include <simple_rt_renderer.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  std::unique_ptr<RtRenderer> renderer(new PointShadowsRtRenderer());
  // std::unique_ptr<RtRenderer> renderer(new SimpleRtRenderer());
  GLFWwindow* window = renderer->OpenWindow("RT Render");

  // load models
  // -----------
/*  {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "lost_empire";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir).append(model_dir).append("/")
      .append(object_file).append(file_type);
    renderer->AddModel(file_path);
    }*/
  {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "nanosuit";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir).append(model_dir).append("/")
      .append(object_file).append(file_type);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.0f, 0.0f));
    model_mat = glm::scale(model_mat, glm::vec3(0.1f, 0.1f, 0.1f)); // scale nanosuit model
    renderer->AddModel(file_path, model_mat);
  }
    {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "sponza";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir).append(model_dir).append("/")
      .append(object_file).append(file_type);
    glm::mat4 model_mat = glm::mat4(1.0f);
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.0f, 0.0f));
    model_mat = glm::scale(model_mat, glm::vec3(0.005f, 0.005f, 0.005f)); // scale sponza model
    renderer->AddModel(file_path, model_mat);
  }
    
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    renderer->Render();
  }

  glfwTerminate();
  return 0;
}
