#include "tracer/intersectable.hpp"

#include <algorithm>

#include "interpolation.hpp"

std::optional<DVec3> IntersectTri(const Ray& ray, DVec3 verts[3]) {
  double eps = epsilon(verts[0]);
  DVec3 edge0 = verts[1] - verts[0];
  DVec3 edge1 = verts[2] - verts[0];
  DVec3 p = glm::cross(ray.dir, edge1);

  double determinant = dot(edge0, p);
  if (determinant > -eps && determinant < eps) {
    return std::nullopt;
  }
  double invDeterminant = 1 / determinant;

  DVec3 vertToOrigin = ray.origin - verts[0];

  double u = glm::dot(vertToOrigin, p) * invDeterminant;
  if (u < 0 || u > 1) return std::nullopt;

  DVec3 q = glm::cross(vertToOrigin, edge0);
  double v = glm::dot(ray.dir, q) * invDeterminant;
  if (v < 0 || u + v > 1) return std::nullopt;

  double t = glm::dot(edge1, q) * invDeterminant;

  if (t > eps) {
    return ray.origin + t * ray.dir;
  }
  return std::nullopt;
}

AaBox::AaBox(DVec3 bot, DVec3 top) {
  this->bot = bot;
  this->top = top;
}

AaBox::AaBox(DVec3 point) {
  this->bot = point;
  this->top = point;
}

std::optional<InterPoint> AaBox::Intersect(const Ray& ray) {
  return std::nullopt;
}

AaBox AaBox::GetAaBox() { return *this; }

void AaBox::Update(DVec3 point) {
  for (int i = 0; i < 3; i++) {
    bot[i] = std::min(bot[i], point[i]);
  }
  for (int i = 0; i < 3; i++) {
    top[i] = std::max(top[i], point[i]);
  }
}

InterTri::InterTri(Material* material, DVertex vert0, DVertex vert1,
                   DVertex vert2)
    : material_(material) {
  verts_[0] = vert0;
  verts_[1] = vert1;
  verts_[2] = vert2;
}

std::optional<InterPoint> InterTri::Intersect(const Ray& ray) {
  DVec3 verts[3] = {
      verts_[0].Position,
      verts_[1].Position,
      verts_[2].Position,
  };
  std::optional<DVec3> point = IntersectTri(ray, verts);
  if (!point.has_value()) {
    return std::nullopt;
  }
  return InterPoint({*point, this});
}

AaBox InterTri::GetAaBox() {
  AaBox box(verts_[0].Position);
  for (int i = 0; i < 3; i++) {
    box.Update(verts_[i].Position);
  }
  return box;
}

Material* InterTri::material() const { return material_; }

DVec2 InterTri::GetUv(DVec3 point) {
  DVec3 weights = GetBarycentricWeights(verts_[0].Position, verts_[1].Position,
                                        verts_[2].Position, point);
  DVec2 uv;
  for (int i = 0; i < 3; i++) {
    uv += weights[i] * verts_[i].TexCoords;
  }
  return uv;
}
