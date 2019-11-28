#ifndef POINT_SHADOWS_RT_RENDERER_HPP
#define POINT_SHADOWS_RT_RENDERER_HPP

#include <memory>
#include <string>

#include "glitter.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "learnopengl/camera.h"
#include "learnopengl/filesystem.h"
#include "learnopengl/model.h"
#include "learnopengl/shader.h"
#include "rt_renderer.hpp"

class PointShadowsRtRenderer : public RtRenderer {
 public:
  PointShadowsRtRenderer();
  GLFWwindow* OpenWindow(const std::string& window_name = "RT Render") override;
  void AddModel(const std::string& file_path, glm::mat4 model_matrix) override;
  void AddModel(std::unique_ptr<Renderable> model, glm::mat4 model_matrix) override;
  void Render() override;
  bool WindowShouldClose() override;

 private:
  void processInput(float deltaTime);

  GLFWwindow* window_;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Shader> depth_shader_;
  std::unique_ptr<Shader> light_box_shader_;
  std::vector<std::unique_ptr<Renderable>> models_;
  std::vector<glm::mat4> model_matrices_;

  bool pause_ = false;
  bool pause_key_pressed_ = false;

  float lastFrameTime = 0.0f;
  unsigned int depthMapFBO;
  unsigned int depthCubemap;
  glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif
