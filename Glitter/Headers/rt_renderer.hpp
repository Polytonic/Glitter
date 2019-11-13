#ifndef RT_RENDERER_HPP
#define RT_RENDERER_HPP

#include <string>

#include "glitter.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RtRenderer {
public:
  virtual GLFWwindow* OpenWindow(const std::string& window_name) = 0;
  virtual void AddModel(const std::string& file_path,
			glm::mat4 model_matrix) = 0;
  virtual void Render() = 0;
};

#endif
