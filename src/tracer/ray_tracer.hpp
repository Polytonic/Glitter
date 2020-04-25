#ifndef TRACER_RAY_TRACER_HPP
#define TRACER_RAY_TRACER_HPP

#include <memory>
#include <optional>
#include <vector>

#include "learnopengl/camera.h"
#include "learnopengl/mesh.h"
#include "scene/primitives.hpp"
#include "tracer/bound.hpp"
#include "tracer/intersectable.hpp"
#include "tracer/transparency.hpp"

class RayTracer {
 public:
  struct Options {
    RgbPix background_color = {0, 0, 0};
    int max_depth = 8;
  };

  struct RecursiveContext {
    int depth = 0;
    InsideModelStack inside_models;
  };

  struct TransparencyData {
    DVec3 color;
    double absorption_percent;
    DVec3 absorption_color;
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

  virtual DVec3 Shade(const ShadeablePoint& point, const SceneLights& lights,
                      RecursiveContext context);

  virtual DVec3 IntersectAndShade(Ray ray, const SceneLights& lights,
                                  RecursiveContext context);

  virtual DVec3 CalculateReflectionColor(const ShadeablePoint& start_point,
                                         const SceneLights& lights,
                                         RecursiveContext context);
  virtual TransparencyData CalculateRefractionColor(
      const ShadeablePoint& start_point, const SceneLights& lights,
      RecursiveContext context);

  // `view_dir` is the vector from the point to the camera
  virtual DVec3 CalculatePointLight(const ShadeablePoint& point,
                                    const Light& light, DVec3 diffuse_color,
                                    DVec3 specular_color, DVec3 normal);
  virtual DVec3 CalculatePointShadow(DVec3 point, const Light& light);

  virtual DVec3 CalculateDirectionalLight(const ShadeablePoint& point,
                                          DVec3 light_in_dir, DVec3 light_color,
                                          DVec3 diffuse_color,
                                          DVec3 specular_color, DVec3 normal);
  virtual DVec3 CalculateDirectionalShadow(DVec3 point, DVec3 light_in_dir);

  std::vector<InterPtr> inters_;
  BoundPtr outer_bound_;
  Options options_;
};

#endif
