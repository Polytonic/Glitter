#ifndef REALTIME_FPS_COUNTER_HPP
#define REALTIME_FPS_COUNTER_HPP

#include <queue>

#include "shapes/dynamic_renderable.hpp"
#include "shapes/renderable.hpp"

class FpsCounter : public CameraEventHandler {
 public:
  FpsCounter(double delay = 5.0);
  void KeyboardEvents(GLFWwindow* window) override;
  void TickUpdateCamera(Camera* camera, double delta_time) override;

 private:
  std::queue<double> frames_;
  double last_printed_;
  double delay_;
};

#endif
