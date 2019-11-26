#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "glitter.hpp"

struct InterParam {
  float x1;
  float y1;
  float x2;
  float y2;
  float v11;
  float v12;
  float v21;
  float v22;

  std::string DebugString() const;
};

float InterpolateValue(float x1, float x2, float v1, float v2, float x);

float InterpolateValue(InterParam known, glm::vec2 point);

#endif
