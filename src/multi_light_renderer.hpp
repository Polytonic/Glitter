#ifndef MULTI_LIGHT_RENDERER_HPP
#define MULTI_LIGHT_RENDERER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "glitter.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "learnopengl/camera.h"
#include "learnopengl/filesystem.h"
#include "learnopengl/model.h"
#include "learnopengl/shader.h"
#include "rt_renderer.hpp"

class MultiLightRenderer : public RtRenderer {
 public:
  MultiLightRenderer();
  GLFWwindow* OpenWindow(const std::string& window_name = "RT Render") override;
  void AddModel(const std::string& file_path, glm::mat4 model_matrix) override;
  void AddModel(std::unique_ptr<Renderable> model,
                glm::mat4 model_matrix) override;
  void AddDynamicModel(std::unique_ptr<DynamicRenderable> model) override;
  void AddEventHandler(CameraEventHandler* event_handler) override;
  void Render() override;
  bool WindowShouldClose() override;

  bool AddLight(const Light& light);
  int MaxNumLights() const;

 private:
  void processInput(float deltaTime);

  GLFWwindow* window_;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Shader> depth_shader_;
  std::unique_ptr<Shader> light_box_shader_;
  std::vector<std::unique_ptr<Renderable>> static_models_;
  std::vector<glm::mat4> static_model_matrices_;
  std::vector<std::unique_ptr<DynamicRenderable>> dynamic_models_;
  std::vector<CameraEventHandler*> event_handlers_;
  std::unordered_map<int, bool> key_states_;

  bool pause_ = true;

  float lastFrameTime = 0.0f;
  unsigned int depthMapFBO;
  unsigned int depthCubemap;
  std::vector<Light> lights_;
};

#endif
