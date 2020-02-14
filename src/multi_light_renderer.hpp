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
  MultiLightRenderer(bool windowed_mode = true) : RtRenderer(windowed_mode) {}
  GLFWwindow* Init(const std::string& window_name = "RT Render") override;
  void AddEventHandler(CameraEventHandler* event_handler) override;
  void Render() override;
  bool WindowShouldClose() override;
  void MoveCamera(const CameraArrangement& camera);
  const Camera& camera();

  bool AddLight(const Light& light);
  int MaxNumLights() const;

  void set_directional_light_pos(glm::vec3 pos) {
    directional_light_pos_ = pos;
  }

  void set_directional_light_color(glm::vec3 color) {
    directional_light_color_ = color;
  }

 private:
  void processInput(float deltaTime);

  GLFWwindow* window_ = nullptr;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Shader> depth_shader_;
  std::unique_ptr<Shader> light_box_shader_;
  std::vector<CameraEventHandler*> event_handlers_;
  std::unordered_map<int, bool> key_states_;

  bool pause_ = true;

  float lastFrameTime = 0.0f;
  unsigned int depth_map_texture_;
  unsigned int depth_map_fbo_;
  std::vector<Light> lights_;
  glm::vec3 directional_light_pos_ = glm::vec3(-6.0f, 6.0f, -6.0f);
  glm::vec3 directional_light_color_ = glm::vec3(0.2f, 0.2f, 0.2f);
};

#endif
