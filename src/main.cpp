#define _USE_MATH_DEFINES
#include <cmath>

// Local Headers
#include "glitter.hpp"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

// Standard Headers
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>

#include "example_scenes.hpp"
#include "rt_renderer.hpp"

int main() {
  // std::default_random_engine random_gen(time(NULL));
  std::default_random_engine random_gen(4);
  random_gen.discard(64);

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  std::unique_ptr<RtRenderer> renderer = InProgressScene(&random_gen);
  // std::unique_ptr<RtRenderer> renderer(new SimpleRtRenderer());

  // render loop
  // -----------
  std::cerr << "Starting rendering" << std::endl;
  while (!renderer->WindowShouldClose()) {
    renderer->Render();
  }

  glfwTerminate();
  return 0;
}
