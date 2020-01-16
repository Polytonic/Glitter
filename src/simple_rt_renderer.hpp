#ifndef SIMPLE_RT_RENDERER_HPP
#define SIMPLE_RT_RENDERER_HPP

#include <memory>
#include <string>

#include "glitter.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <learnopengl/camera.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader_m.h>
#include <rt_renderer.hpp>

class SimpleRtRenderer : public RtRenderer {
 public:
  SimpleRtRenderer();
  GLFWwindow* OpenWindow(const std::string& window_name = "RT Render") override;
  void AddModel(const std::string& file_path, glm::mat4 model_matrix) override;
  void AddModel(std::unique_ptr<Renderable> model,
                glm::mat4 model_matrix) override;
  void AddDynamicModel(std::unique_ptr<DynamicRenderable> model) override;
  void AddEventHandler(CameraEventHandler* event_handler) override {}
  void Render() override;
  bool WindowShouldClose() override;

 private:
  void processInput(float deltaTime);

  GLFWwindow* window_;
  std::unique_ptr<Shader> shader_;
  std::vector<std::unique_ptr<Renderable>> models_;
  std::vector<glm::mat4> model_matrices_;

  float lastFrameTime = 0.0f;
};

#endif
