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
#include <sstream>
#include <string>

#include "example_scenes.hpp"
#include "learnopengl/filesystem.h"
#include "rt_renderer.hpp"
#include "tracer/acceleration.hpp"
#include "tracer/bound.hpp"
#include "tracer/intersectable.hpp"
#include "tracer/ray_tracer.hpp"

CameraArrangement GetStartingCamera(int argc, char** argv) {
  CameraArrangement camera = {
      .position = glm::vec3(0.0f, 0.0f, 2.0f),
      .view_dir = glm::vec3(0.0f, 0.0f, -1.0f),
  };
  if (argc >= 2) {
    std::stringstream in;
    in << argv[1] << " ";
    float coords[6];
    for (int i = 0; i < 6 && in.good(); i++) {
      in >> coords[i];
    }
    if (in.good()) {
      camera.position = glm::vec3(coords[0], coords[1], coords[2]);
      camera.view_dir = glm::vec3(coords[3], coords[4], coords[5]);
      std::cerr << "Using input camera position" << std::endl;
    }
  }
  return camera;
}

int main(int argc, char** argv) {
  // std::default_random_engine random_gen(time(NULL));
  std::default_random_engine random_gen(4);
  random_gen.discard(64);

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  bool trace = true;

  std::unique_ptr<RtRenderer> renderer = CurrentScene(!trace, &random_gen);

  renderer->MoveCamera(GetStartingCamera(argc, argv));

  if (trace) {
    std::cerr << "Starting ray tracing" << std::endl;
    CameraTracerOpts opts;
    opts.h_px = 100;
    opts.w_px = 100;
    opts.focal_length = 0.01;
    opts.focus_distance = 5;
    opts.vert_fov = 0.785398;
    renderer->SetCameraOpts(opts);
    std::vector<InterPtr> inters;
    renderer->GetTris(&inters);
    RayTracer::Options t_opts = {
        .background_color = {100, 100, 100},
    };
    std::unique_ptr<RayTracer> tracer =
        // RayTracer::CreateNoAcceleration(t_opts, std::move(inters));
        RayTracer::CreateTopDownTriple(t_opts, std::move(inters));
    Texture tex = tracer->Render(renderer->camera());
    TextureToFile("output.png", tex);
  }

  std::cerr << "Starting rendering" << std::endl;
  while (!renderer->WindowShouldClose()) {
    renderer->Render();
  }

  glfwTerminate();
  const Camera& camera = renderer->camera();
  std::cout << "Camera:" << std::endl;
  std::cout << camera.position().x << " " << camera.position().y << " "
            << camera.position().z << " " << camera.front().x << " "
            << camera.front().y << " " << camera.front().z << std::endl;
  return 0;
}
