#include "shapes/interpolation.hpp"

#include <iostream>
#include <sstream>

std::string InterParam::DebugString() const {
  std::ostringstream stream;
  stream << "x1: " << x1 << "y1: " << y1 << "x2: " << x2 << "y2: " << y2
         << "\nv11: " << v11 << "\nv12: " << v12 << "\nv21: " << v21
         << "\nv22: " << v22;
  return stream.str();
}

double InterpolateValue(double x1, double x2, double v1, double v2, double x) {
  if (x1 == x2) {
    return (v1 + v2) / 2.0;
  }
  return (((x2 - x) / (x2 - x1)) * v1) + (((x - x1) / (x2 - x1)) * v2);
}

double InterpolateValue(InterParam known, DVec2 point) {
  if (known.x1 == known.x2 && known.y1 == known.y2) {
    return (known.v11 + known.v12 + known.v21 + known.v22) / 4.0;
  }
  if (known.x1 == known.x2) {
    return InterpolateValue(known.y1, known.y2, (known.v11 + known.v21) / 2.0,
                            (known.v12 + known.v22) / 2.0, point.y);
  }
  if (known.y1 == known.y2) {
    return InterpolateValue(known.x1, known.x2, (known.v11 + known.v12) / 2.0,
                            (known.v21 + known.v22) / 2.0, point.x);
  }
  double inv = 1.0 / ((known.x2 - known.x1) * (known.y2 - known.y1));
  DVec2 x_vec = {known.x2 - point.x, point.x - known.x1};
  DVec2 y_vec = {known.y2 - point.y, point.y - known.y1};
  // glm uses column-first order.
  DMat2 val_mat(known.v11, known.v21, known.v12, known.v22);
  return inv * glm::dot(x_vec, val_mat * y_vec);
}

DVec3 GetBarycentricWeights(const DVec3& p1, const DVec3& p2, const DVec3& p3,
                            const DVec3& point) {
  DVec3 res(0);
  double sideLengths[3] = {glm::distance(p1, p2), glm::distance(p2, p3),
                           glm::distance(p3, p1)};
  const DVec3* verts[4] = {&p1, &p2, &p3, &p1};

  for (int i = 0; i < 3; i++) {
    double a = sideLengths[i];
    double b = glm::distance(*verts[i], point);
    double c = glm::distance(*verts[i + 1], point);
    double p = (a + b + c) / 2.0;
    // res[i] = std::max(std::sqrt(p * (p - a) * (p - b) * (p - c)), 0.0);
    res[i] = std::sqrt(p * (p - a) * (p - b) * (p - c));
  }
  double sum = res.x + res.y + res.z;
  res *= 1.0 / sum;
  return {res.y, res.z, res.x};
}
