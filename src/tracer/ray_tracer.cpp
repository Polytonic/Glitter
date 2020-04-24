#include "tracer/ray_tracer.hpp"

#include "GLFW/glfw3.h"

#include "texture/tex_canvas.hpp"
#include "texture/texture_gen.hpp"
#include "tracer/acceleration.hpp"

std::unique_ptr<RayTracer> RayTracer::CreateNoAcceleration(
    Options options, std::vector<InterPtr> inters) {
  double start = glfwGetTime();
  BoundPtr outer_bound = ConstructBoundsNoAcceleration(&inters);
  double elapsed = glfwGetTime() - start;
  std::cerr << "Acceleration time: " << elapsed << std::endl;
  return std::unique_ptr<RayTracer>(
      new RayTracer(options, std::move(inters), std::move(outer_bound)));
}

std::unique_ptr<RayTracer> RayTracer::CreateTopDownTriple(
    Options options, std::vector<InterPtr> inters) {
  double start = glfwGetTime();
  BoundPtr outer_bound =
      ConstructBoundsTopDownTriple(BoundTopDownTripleOptions(), &inters);
  double elapsed = glfwGetTime() - start;
  std::cerr << "Acceleration time: " << elapsed << std::endl;
  return std::unique_ptr<RayTracer>(
      new RayTracer(options, std::move(inters), std::move(outer_bound)));
}

RayTracer::RayTracer(Options options, std::vector<InterPtr> inters,
                     BoundPtr outer_bound)
    : options_(std::move(options)),
      inters_(std::move(inters)),
      outer_bound_(std::move(outer_bound)) {}

Texture RayTracer::Render(Camera camera, const SceneLights& scene_lights) {
  SceneLights lights = scene_lights;
  if (lights.directional_light_in_dir.has_value()) {
    lights.directional_light_in_dir =
        glm::normalize(*lights.directional_light_in_dir);
  }
  double start = glfwGetTime();
  TexCanvas canvas = GetColorCanvas(options_.background_color,
                                    camera.opts().w_px, camera.opts().h_px);
  outer_bound_->RecursiveAssertSanity();
  for (int y = 0; y < camera.opts().h_px; y++) {
    for (int x = 0; x < camera.opts().w_px; x++) {
      std::vector<Ray> pix_rays = camera.GetScreenRays(x, y);
      for (Ray ray : pix_rays) {
        std::optional<ShadeablePoint> point = IntersectScene(ray);
        if (point.has_value()) {
          RecursiveContext context;
          RgbPix color =
              RgbPix::Convert(Shade(*point, camera, lights, context));
          canvas.SetPix(x, y, color);
        }
      }
    }
  }
  double elapsed = glfwGetTime() - start;
  std::cerr << "Render time: " << elapsed << std::endl;
  return canvas.ToTexture();
}

/*std::optional<ShadeablePoint> RayTracer::IntersectScene(Ray ray) {
  ShadeablePoint closest = {DVec3(0), nullptr};
  double closest_dist2 = 1e50;
  for (InterPtr& bound : inters_) {
    std::optional<ShadeablePoint> intersection = bound->Intersect(ray);
    if (intersection.has_value() && intersection->shape->IsShadeable()) {
      double dist2 = glm::distance2(intersection->point, ray.origin);
      if (dist2 < closest_dist2) {
        closest_dist2 = dist2;
        closest = *intersection;
      }
    }
  }
  if (closest.shape == nullptr) {
    return std::nullopt;
  }
  return closest;
  }*/

std::optional<ShadeablePoint> RayTracer::IntersectScene(Ray ray) {
  return outer_bound_->Intersect(ray);
}

DVec3 RayTracer::Shade(const ShadeablePoint& point, const Camera& camera,
                       const SceneLights& lights, RecursiveContext context) {
  DVec3 view_dir = glm::normalize(camera.Position - point.point);
  DVec3 diffuse = point.shape->material()
                      ->diff_texture()
                      .Sample(point.shape->GetUv(point.point))
                      .ToFloat();
  if (!point.shape->material()->apply_shading()) {
    return diffuse;
  }
  DVec3 specular = diffuse;
  DVec3 normal = point.shape->GetNormal(point.point);
  DVec3 direct_lighting(0.0);
  // Hard-coded ambient light
  direct_lighting += 0.1 * diffuse;
  if (lights.directional_light_in_dir.has_value()) {
    direct_lighting += CalculateDirectionalLight(
        point, view_dir, *lights.directional_light_in_dir,
        lights.directional_light_color, diffuse, specular, normal);
  }
  for (const Light& light : lights.points) {
    direct_lighting +=
        CalculatePointLight(point, view_dir, light, diffuse, specular, normal);
  }
  double direct_lighting_component_strength =
      std::max(0.0, 1.0 - (point.shape->material()->options().transparency +
                           point.shape->material()->options().reflectivity));

  DVec3 total_lighting = direct_lighting_component_strength * direct_lighting;

  if (point.shape->material()->is_reflective()) {
    DVec3 reflection_color =
        CalculateReflectionColor(point, camera, lights, context);
    total_lighting +=
        reflection_color * point.shape->material()->options().reflectivity;
  }

  return total_lighting;
}

DVec3 RayTracer::CalculateReflectionColor(const ShadeablePoint& start_point,
                                          const Camera& camera,
                                          const SceneLights& lights,
                                          RecursiveContext context) {
  context.depth += 1;
  if (context.depth > options_.max_depth) {
    return DVec3(0.0);
  }

  DVec3 normal = start_point.shape->GetNormal(start_point.point);
  Ray ray = {
      .origin = start_point.point,
      .dir = glm::normalize(glm::reflect(start_point.ray.dir, normal)),
  };

  std::optional<ShadeablePoint> point = IntersectScene(ray);
  if (point.has_value()) {
    return Shade(*point, camera, lights, context);
  }
  return options_.background_color.ToFloat();
}

DVec3 RayTracer::CalculatePointLight(const ShadeablePoint& point,
                                     DVec3 view_dir, const Light& light,
                                     DVec3 diffuse_color, DVec3 specular_color,
                                     DVec3 normal) {
  DVec3 point_shadow = DVec3(1.0) - CalculatePointShadow(point.point, light);
  DVec3 light_color(light.Color);
  if (point_shadow == DVec3(0.0)) {
    return DVec3(0.0);
  }
  DVec3 light_out_dir = DVec3(light.Position) - point.point;
  double light_distance = glm::length(light_out_dir);
  light_out_dir = glm::normalize(light_out_dir);
  DVec3 diff_light = std::max(glm::dot(normal, light_out_dir), 0.0) *
                     diffuse_color * light_color;
  DVec3 halfway_dir = glm::normalize(light_out_dir + view_dir);
  double spec_strength =
      std::pow(std::max(glm::dot(normal, halfway_dir), 0.0), 16.0);
  DVec3 spec_light = light_color * specular_color * spec_strength;
  double attenuation =
      1.0 / (1.0 + light.Linear * light_distance +
             light.Quadratic * light_distance * light_distance);
  DVec3 lighting(0.0);
  lighting += attenuation * point_shadow * diff_light;
  lighting += attenuation * point_shadow * spec_light;
  return lighting;
}

DVec3 RayTracer::CalculatePointShadow(DVec3 point, const Light& light) {
  DVec3 light_position(light.Position);
  Ray out_ray = {
      .origin = point,
      .dir = glm::normalize(light_position - point),
  };
  EpsilonAdvance(&out_ray);
  std::optional<ShadeablePoint> intersection = IntersectScene(out_ray);
  if (intersection.has_value()) {
    double light_dist = glm::distance(point, light_position);
    if (glm::distance(intersection->point, point) >= light_dist) {
      // Intersected object is farther away than light.
      return DVec3(0.0);
    } else {
      return DVec3(1.0);
    }
  } else {
    // Hit nothing, no shadow
    return DVec3(0.0);
  }
}

DVec3 RayTracer::CalculateDirectionalLight(const ShadeablePoint& point,
                                           DVec3 view_dir, DVec3 light_in_dir,
                                           DVec3 light_color,
                                           DVec3 diffuse_color,
                                           DVec3 specular_color, DVec3 normal) {
  DVec3 directional_shadow =
      DVec3(1.0) - CalculateDirectionalShadow(point.point, light_in_dir);
  if (directional_shadow == DVec3(0.0)) {
    return DVec3(0.0);
  }
  DVec3 light_out_dir = glm::normalize(-1.0 * light_in_dir);
  DVec3 diff_light = std::max(glm::dot(normal, light_out_dir), 0.0) *
                     diffuse_color * light_color;
  DVec3 halfway_dir = glm::normalize(light_out_dir + view_dir);
  double spec_strength =
      std::pow(std::max(glm::dot(normal, halfway_dir), 0.0), 16.0);
  DVec3 spec_light = light_color * specular_color * spec_strength;
  DVec3 lighting(0.0);
  lighting += directional_shadow * diff_light;
  lighting += directional_shadow * spec_light;
  return lighting;
}

DVec3 RayTracer::CalculateDirectionalShadow(DVec3 point, DVec3 light_in_dir) {
  Ray out_ray = {
      .origin = point,
      .dir = glm::normalize(-1.0 * light_in_dir),
  };
  EpsilonAdvance(&out_ray);
  // If it hit something, full shadow, otherwise none.
  return IntersectScene(out_ray).has_value() ? DVec3(1.0) : DVec3(0.0);
}
