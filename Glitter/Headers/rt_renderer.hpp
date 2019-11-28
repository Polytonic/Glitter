#ifndef RT_RENDERER_HPP
#define RT_RENDERER_HPP

#include <string>

#include "glitter.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "dynamic_renderable.hpp"
#include "renderable.hpp"

class RtRenderer {
 public:
  virtual GLFWwindow* OpenWindow(const std::string& window_name) = 0;
  virtual void AddModel(const std::string& file_path,
                        glm::mat4 model_matrix) = 0;
  virtual void AddModel(std::unique_ptr<Renderable> model,
                        glm::mat4 model_matrix) = 0;
  virtual void AddDynamicModel(std::unique_ptr<DynamicRenderable> model) = 0;
  virtual void Render() = 0;
  virtual bool WindowShouldClose() = 0;
};

#endif
