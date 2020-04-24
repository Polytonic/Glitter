#ifndef REALTIME_RT_RENDERER_HPP
#define REALTIME_RT_RENDERER_HPP

#include <memory>
#include <string>
#include <vector>

#include "learnopengl/glitter.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "shapes/dynamic_renderable.hpp"
#include "learnopengl/camera.h"
#include "learnopengl/model.h"
#include "shapes/renderable.hpp"

class RtRenderer {
 public:
  RtRenderer(bool windowed_mode = true) : windowed_mode_(windowed_mode) {}
  virtual GLFWwindow* Init(const std::string& window_name) = 0;
  virtual void AddModel(const std::string& file_path, glm::mat4 model_matrix);
  virtual void AddModel(std::unique_ptr<Renderable> model,
                        glm::mat4 model_matrix);
  virtual void AddDynamicModel(std::unique_ptr<DynamicRenderable> model);
  virtual void GetTris(std::vector<InterPtr>* tris);
  virtual void AddEventHandler(CameraEventHandler* event_handler) = 0;
  virtual void Render() = 0;
  virtual bool WindowShouldClose() = 0;
  virtual void MoveCamera(const CameraArrangement& camera) = 0;
  virtual const Camera& camera() = 0;
  virtual void SetCameraOpts(CameraTracerOpts opts) = 0;
  virtual SceneLights GetLights() const = 0;

 protected:
  bool windowed_mode_;

  std::vector<std::unique_ptr<Renderable>> static_models_;
  std::vector<glm::mat4> static_model_matrices_;
  std::vector<std::unique_ptr<DynamicRenderable>> dynamic_models_;
};

#endif
