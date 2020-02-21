#include "tracer/ray_tracer.hpp"

#include "tracer/acceleration.hpp"
#include "tex_canvas.hpp"
#include "texture_gen.hpp"

std::unique_ptr<RayTracer> RayTracer::Create(Options options,
                                             std::vector<InterPtr> inters) {
  BoundPtr outer_bound = ConstructBounds(&inters);
  return std::unique_ptr<RayTracer>(
      new RayTracer(options, std::move(inters), std::move(outer_bound)));
}

RayTracer::RayTracer(Options options, std::vector<InterPtr> inters,
                     BoundPtr outer_bound)
    : options_(std::move(options)),
      inters_(std::move(inters)),
      outer_bound_(std::move(outer_bound)) {}

Texture RayTracer::Render(Camera camera) {
  TexCanvas canvas = GetColorCanvas(options_.background_color,
                                    camera.opts().w_px, camera.opts().h_px);
  for (int y = 0; y < camera.opts().h_px; y++) {
    for (int x = 0; x < camera.opts().w_px; x++) {
      std::vector<Ray> pix_rays = camera.GetScreenRays(x, y);
      for (Ray ray : pix_rays) {
        std::optional<ShadeablePoint> point = IntersectScene(ray);
        if (point.has_value()) {
          RgbPix color = Shade(*point);
          canvas.SetPix(x, y, color);
        }
      }
    }
  }
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

RgbPix RayTracer::Shade(ShadeablePoint point) {
  return point.shape->material()->diff_texture().Sample(
      point.shape->GetUv(point.point));
}
