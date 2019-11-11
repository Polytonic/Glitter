// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

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

  std::unique_ptr<SimpleRtRenderer> renderer(new SimpleRtRenderer());
  GLFWwindow* window = renderer->OpenWindow();

  // load models
  // -----------
  {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "lost_empire";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir).append(model_dir).append("/")
      .append(object_file).append(file_type);
    renderer->AddModel(file_path);
  }
  {
    std::string objects_dir = "resources/objects/";
    std::string model_dir = "nanosuit";
    std::string object_file = model_dir;
    std::string file_type = ".obj";
    std::string file_path;
    file_path.append(objects_dir).append(model_dir).append("/")
      .append(object_file).append(file_type);
    renderer->AddModel(file_path);
  }
    
  // draw in wireframe
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    renderer->Render();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}
