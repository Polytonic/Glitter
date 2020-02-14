#ifndef RT_RENDERER_HPP
#define RT_RENDERER_HPP

#include <string>

#include "glitter.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "dynamic_renderable.hpp"
#include "learnopengl/camera.h"
#include "renderable.hpp"

class RtRenderer {
 public:
  RtRenderer(bool windowed_mode = true) : windowed_mode_(windowed_mode) {}
  virtual GLFWwindow* Init(const std::string& window_name) = 0;
  virtual void AddModel(const std::string& file_path,
                        glm::mat4 model_matrix) = 0;
  virtual void AddModel(std::unique_ptr<Renderable> model,
                        glm::mat4 model_matrix) = 0;
  virtual void AddDynamicModel(std::unique_ptr<DynamicRenderable> model) = 0;
  virtual void AddEventHandler(CameraEventHandler* event_handler) = 0;
  virtual void Render() = 0;
  virtual bool WindowShouldClose() = 0;
  virtual void MoveCamera(const CameraArrangement& camera) = 0;
  virtual const Camera& camera() = 0;

 protected:
  bool windowed_mode_;
};

#endif
