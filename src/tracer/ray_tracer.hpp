#ifndef TRACER_RAY_TRACER_HPP
#define TRACER_RAY_TRACER_HPP

#include <memory>
#include <optional>
#include <vector>

#include "learnopengl/camera.h"
#include "learnopengl/mesh.h"
#include "tracer/bound.hpp"
#include "tracer/intersectable.hpp"

class RayTracer {
 public:
  struct Options {
    RgbPix background_color = {0, 0, 0};
  };

  static std::unique_ptr<RayTracer> CreateNoAcceleration(
      Options options, std::vector<InterPtr> inters);
  static std::unique_ptr<RayTracer> CreateTopDownTriple(
      Options options, std::vector<InterPtr> inters);
  virtual Texture Render(Camera camera);

 protected:
  RayTracer(Options options, std::vector<InterPtr> inters,
            BoundPtr outer_bounds);

  virtual std::optional<ShadeablePoint> IntersectScene(Ray ray);

  virtual RgbPix Shade(ShadeablePoint point);

  std::vector<InterPtr> inters_;
  BoundPtr outer_bound_;
  Options options_;
};

#endif
