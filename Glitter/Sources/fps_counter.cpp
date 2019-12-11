#include "fps_counter.hpp"

#include <GLFW/glfw3.h>
#include "glitter.hpp"

void FpsCounter::KeyboardEvents(GLFWwindow* window) {}

void FpsCounter::TickUpdateCamera(Camera* camera, double delta_time) {
  double now = glfwGetTime();
  frames_.push(now);
  if (frames_.empty()) {
    last_printed_ = now;
    return;
  }
  if (now > last_printed_ + 1) {
    while (frames_.front() < now - 5) {
      frames_.pop();
    }
    double fps = frames_.size() / (now - frames_.front());
    last_printed_ = now;
    std::cout << "FPS: " << fps << std::endl;
  }
}
