#include "interpolation.hpp"

#include <iostream>
#include <sstream>

std::string InterParam::DebugString() const {
  std::ostringstream stream;
  stream << "x1: " << x1 << "y1: " << y1 << "x2: " << x2 << "y2: " << y2
         << "\nv11: " << v11 << "\nv12: " << v12 << "\nv21: " << v21
         << "\nv22: " << v22;
  return stream.str();
}

float InterpolateValue(float x1, float x2, float v1, float v2, float x) {
  if (x1 == x2) {
    return (v1 + v2) / 2.0f;
  }
  return (((x2 - x) / (x2 - x1)) * v1) + (((x - x1) / (x2 - x1)) * v2);
}

float InterpolateValue(InterParam known, glm::vec2 point) {
  if (known.x1 == known.x2 && known.y1 == known.y2) {
    return (known.v11 + known.v12 + known.v21 + known.v22) / 4.0f;
  }
  if (known.x1 == known.x2) {
    return InterpolateValue(known.y1, known.y2, (known.v11 + known.v21) / 2.0f,
                            (known.v12 + known.v22) / 2.0f, point.y);
  }
  if (known.y1 == known.y2) {
    return InterpolateValue(known.x1, known.x2, (known.v11 + known.v12) / 2.0f,
                            (known.v21 + known.v22) / 2.0f, point.x);
  }
  float inv = 1.0f / ((known.x2 - known.x1) * (known.y2 - known.y1));
  glm::vec2 x_vec = {known.x2 - point.x, point.x - known.x1};
  glm::vec2 y_vec = {known.y2 - point.y, point.y - known.y1};
  // glm uses column-first order.
  glm::mat2 val_mat(known.v11, known.v21, known.v12, known.v22);
  return inv * glm::dot(x_vec, val_mat * y_vec);
}
