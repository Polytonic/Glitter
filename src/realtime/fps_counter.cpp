#include "realtime/fps_counter.hpp"

#include <GLFW/glfw3.h>
#include "learnopengl/glitter.hpp"

FpsCounter::FpsCounter(double delay) : delay_(delay) {}

void FpsCounter::KeyboardEvents(GLFWwindow* window) {}

void FpsCounter::TickUpdateCamera(Camera* camera, double delta_time) {
  double now = glfwGetTime();
  frames_.push(now);
  if (frames_.empty()) {
    last_printed_ = now;
    return;
  }
  if (now > last_printed_ + delay_) {
    while (frames_.front() < now - 5) {
      frames_.pop();
    }
    double fps = frames_.size() / (now - frames_.front());
    last_printed_ = now;
    std::cerr << "FPS: " << fps << std::endl;
    std::cerr << "Camera:" << camera->position().x << " "
              << camera->position().y << " " << camera->position().z << " "
              << camera->front().x << " " << camera->front().y << " "
              << camera->front().z << std::endl;
  }
}
