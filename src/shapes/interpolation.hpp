#ifndef SHAPES_INTERPOLATION_HPP
#define SHAPES_INTERPOLATION_HPP

#include "learnopengl/glitter.hpp"

struct InterParam {
  double x1;
  double y1;
  double x2;
  double y2;
  double v11;
  double v12;
  double v21;
  double v22;

  std::string DebugString() const;
};

double InterpolateValue(double x1, double x2, double v1, double v2, double x);

double InterpolateValue(InterParam known, DVec2 point);

DVec3 GetBarycentricWeights(const DVec3& p1, const DVec3& p2, const DVec3& p3,
                            const DVec3& point);

#endif
