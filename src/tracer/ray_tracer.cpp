#include "tracer/ray_tracer.hpp"

#include <iostream>

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
          RgbPix color = RgbPix::Convert(Shade(*point, lights, context));
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

DVec3 RayTracer::Shade(const ShadeablePoint& point, const SceneLights& lights,
                       RecursiveContext context) {
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
        point, *lights.directional_light_in_dir, lights.directional_light_color,
        diffuse, specular, normal);
  }
  for (const Light& light : lights.points) {
    direct_lighting +=
        CalculatePointLight(point, light, diffuse, specular, normal);
  }
  double direct_lighting_component_strength =
      std::max(0.0, 1.0 - (point.shape->material()->options().transparency +
                           point.shape->material()->options().reflectivity));

  DVec3 total_lighting = direct_lighting_component_strength * direct_lighting;

  if (point.shape->material()->is_reflective()) {
    DVec3 reflection_color = CalculateReflectionColor(point, lights, context);
    total_lighting +=
        reflection_color * point.shape->material()->options().reflectivity;
  }

  /*
    if (point.shape->material()->is_transparent()) {
      DVec3 refraction_color = CalculateRefractionColor(point, lights, context);
      total_lighting +=
          refraction_color * point.shape->material()->options().transparency;
    }
    */

  return total_lighting;
}

DVec3 RayTracer::IntersectAndShade(Ray ray, const SceneLights& lights,
                                   RecursiveContext context) {
  std::optional<ShadeablePoint> point = IntersectScene(ray);
  if (point.has_value()) {
    return Shade(*point, lights, context);
  } else {
    return options_.background_color.ToFloat();
  }
}

DVec3 RayTracer::CalculateReflectionColor(const ShadeablePoint& start_point,
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
  return IntersectAndShade(ray, lights, context);
}

RayTracer::TransparencyData RayTracer::CalculateRefractionColor(
    const ShadeablePoint& start_point, const SceneLights& lights,
    RecursiveContext context) {
  context.depth += 1;
  if (context.depth > options_.max_depth) {
    return {DVec3(0.0), 0.0, DVec3(0.0)};
  }

  DVec3 normal = start_point.shape->GetNormal(start_point.point);
  double normal_dot = glm::dot(start_point.ray.dir, normal);

  if (normal_dot > 0) {
    // We are coming out of the object.
    if (context.inside_models.Contains(start_point.shape->GetParentModel())) {
      // We were already inside the object, as expected.
      // Calculate light absorption and refract.
      Material* previous_material = context.inside_models.CurrentMaterial();
      std::optional<ShadeablePoint> entry_point =
          context.inside_models.Pop(start_point.shape->GetParentModel());
      Material* next_material = context.inside_models.CurrentMaterial();
      if (!entry_point.has_value()) {
        std::cerr << "Error in CalculateRefractionColor; Contains() and Pop() "
                     "are not in agreement"
                  << std::endl;
        exit(-1);
      }
      DVec3 new_vector = glm::normalize(glm::refract(
          glm::normalize(start_point.ray.dir), -1.0 * normal,
          previous_material->options().index / next_material->options().index));

      double new_normal_dot = glm::dot(new_vector, normal);
      if (new_normal_dot >= 0) {
        // No total internal refraction, proceed as planned.
      } else {
        // Total internal refraction occurred, push back onto the stack.
        context.inside_models.Push(start_point);
      }

      Ray ray = {
          .origin = start_point.point + new_vector * epsilon(start_point.point),
          .dir = new_vector,
      };

      // Calculate light absorption over the duration of the shape.
      Material* absorbing_material = entry_point->shape->material();
      double absorption_percent =
          absorbing_material->options().absorption_per_unit *
          glm::distance(entry_point->point, start_point.point);
      if (absorption_percent >= 1.0) {
        // All light was absorbed
        return {DVec3(0.0), 1.0,
                absorbing_material->options().absorption_color};
      } else {
        DVec3 result_color = IntersectAndShade(ray, lights, context);
        return {result_color, absorption_percent,
                absorbing_material->options().absorption_color};
      }
    } else {
      // We are coming out of an object that we never entered.
      // Refract light but do not calculate light impedence.
      Material* previous_material = context.inside_models.CurrentMaterial();
      Material* next_material = start_point.shape->material();
      DVec3 new_vector = glm::normalize(glm::refract(
          glm::normalize(start_point.ray.dir), -1.0 * normal,
          previous_material->options().index / next_material->options().index));

      double new_normal_dot = glm::dot(new_vector, normal);
      if (new_normal_dot >= 0) {
        // No total internal refraction, proceed as planned.
      } else {
        // Total internal refraction occurred, push onto the stack.
        context.inside_models.Push(start_point);
      }

      Ray ray = {
          .origin = start_point.point + new_vector * epsilon(start_point.point),
          .dir = new_vector,
      };
      return {IntersectAndShade(ray, lights, context), 0.0, DVec3(0.0)};
    }
  } else {
    // We are coming into the object.
    if (context.inside_models.Contains(start_point.shape->GetParentModel())) {
      // We are "entering" a model we were already inside. Continue the original
      // ray unchanged.
      Ray ray = {
          .origin = start_point.point +
                    epsilon(start_point.point) * start_point.ray.dir,
          .dir = start_point.ray.dir,
      };
      return {IntersectAndShade(ray, lights, context), 0.0, DVec3(0.0)};
    } else {
      // We are entering a new object.
      // Refract light and push to the context object.
      Material* previous_material = context.inside_models.CurrentMaterial();
      Material* next_material = start_point.shape->material();
      DVec3 new_vector = glm::normalize(glm::refract(
          glm::normalize(start_point.ray.dir), normal,
          previous_material->options().index / next_material->options().index));
      double new_normal_dot = glm::dot(new_vector, normal);
      if (new_normal_dot >= 0) {
        // A total external? refraction occurred. We do not push the object into
        // the stack.
      } else {
        context.inside_models.Push(start_point);
      }
      Ray ray = {
          .origin = start_point.point + new_vector * epsilon(start_point.point),
          .dir = new_vector,
      };
      return {IntersectAndShade(ray, lights, context), 0.0, DVec3(0.0)};
    }
  }
}

DVec3 RayTracer::CalculatePointLight(const ShadeablePoint& point,
                                     const Light& light, DVec3 diffuse_color,
                                     DVec3 specular_color, DVec3 normal) {
  DVec3 view_dir = glm::normalize(-1.0 * point.ray.dir);
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
                                           DVec3 light_in_dir,
                                           DVec3 light_color,
                                           DVec3 diffuse_color,
                                           DVec3 specular_color, DVec3 normal) {
  DVec3 view_dir = glm::normalize(-1.0 * point.ray.dir);
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
