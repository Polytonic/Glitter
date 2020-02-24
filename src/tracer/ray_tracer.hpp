#ifndef TRACER_RAY_TRACER_HPP
#define TRACER_RAY_TRACER_HPP

#include <memory>
#include <optional>
#include <vector>

#include "learnopengl/camera.h"
#include "learnopengl/mesh.h"
#include "primitives.hpp"
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
  virtual Texture Render(Camera camera, const SceneLights& scene_lights);

 protected:
  RayTracer(Options options, std::vector<InterPtr> inters,
            BoundPtr outer_bounds);

  virtual std::optional<ShadeablePoint> IntersectScene(Ray ray);

  virtual RgbPix Shade(const ShadeablePoint& point, const Camera& camera,
                       const SceneLights& lights);

  // `view_dir` is the vector from the point to the camera
  virtual DVec3 CalculatePointLight(const ShadeablePoint& point, DVec3 view_dir,
                                    const Light& light, DVec3 diffuse_color,
                                    DVec3 specular_color, DVec3 normal);
  virtual DVec3 CalculatePointShadow(DVec3 point, const Light& light);

  virtual DVec3 CalculateDirectionalLight(const ShadeablePoint& point,
                                          DVec3 view_dir, DVec3 light_in_dir,
                                          DVec3 light_color,
                                          DVec3 diffuse_color,
                                          DVec3 specular_color, DVec3 normal);
  virtual DVec3 CalculateDirectionalShadow(DVec3 point, DVec3 light_in_dir);

  std::vector<InterPtr> inters_;
  BoundPtr outer_bound_;
  Options options_;
};

#endif
