#ifndef REALTIME_POINT_SHADOWS_DYNAMIC_RENDERER_HPP
#define REALTIME_POINT_SHADOWS_DYNAMIC_RENDERER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "learnopengl/glitter.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "learnopengl/camera.h"
#include "learnopengl/filesystem.h"
#include "learnopengl/model.h"
#include "learnopengl/shader.h"
#include "realtime/rt_renderer.hpp"

class PointShadowsDynamicRenderer : public RtRenderer {
 public:
  PointShadowsDynamicRenderer(bool windowed_mode = true)
      : RtRenderer(windowed_mode) {}
  GLFWwindow* Init(const std::string& window_name = "RT Render") override;
  void AddEventHandler(CameraEventHandler* event_handler) override;
  void Render() override;
  bool WindowShouldClose() override;
  void MoveCamera(const CameraArrangement& camera);
  const Camera& camera();
  void SetCameraOpts(CameraTracerOpts opts) override;
  SceneLights GetLights() const override;

 private:
  void processInput(float deltaTime);

  GLFWwindow* window_;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Shader> depth_shader_;
  std::unique_ptr<Shader> light_box_shader_;
  std::vector<CameraEventHandler*> event_handlers_;
  std::unordered_map<int, bool> key_states_;

  bool pause_ = true;

  float lastFrameTime = 0.0f;
  unsigned int depthMapFBO;
  unsigned int depthCubemap;
  glm::vec3 lightPos = glm::vec3(0.0f);
};

#endif
