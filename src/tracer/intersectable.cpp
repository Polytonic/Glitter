#include "tracer/intersectable.hpp"

#include <algorithm>

#include "interpolation.hpp"

DVec3 PreventZero(DVec3 vec) {
  if (vec.x == 0) vec.x = epsilon(vec);
  if (vec.y == 0) vec.y = epsilon(vec);
  if (vec.z == 0) vec.z = epsilon(vec);
  return vec;
}

std::optional<DVec3> IntersectTri(Ray ray, DVec3 verts[3]) {
  ray.dir = glm::normalize(ray.dir);
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
  this->bot_ = bot;
  this->top_ = top;
  this->Update(bot);
  this->Update(top);
}

AaBox::AaBox(DVec3 point) {
  this->bot_ = point;
  this->top_ = point;
}

std::optional<ShadeablePoint> AaBox::Intersect(const Ray& ray) {
  return std::nullopt;
}

std::optional<DVec3> AaBox::EarliestIntersect(const Ray& ray) {
  Ray r = ray;
  r.dir = glm::normalize(PreventZero(r.dir));
  DVec3 closePlanes(0);
  DVec3 farPlanes(0);
  DVec3 invDir(1.0 / r.dir.x, 1.0 / r.dir.y, 1.0 / r.dir.z);
  for (int i = 0; i < 3; i++) {
    if (invDir[i] < 0) {
      closePlanes[i] = top_[i];
      farPlanes[i] = bot_[i];
    } else {
      closePlanes[i] = bot_[i];
      farPlanes[i] = top_[i];
    }
  }

  double tMin = (closePlanes.x - r.origin.x) * invDir.x;
  double tMax = (farPlanes.x - r.origin.x) * invDir.x;
  double tyMin = (closePlanes.y - r.origin.y) * invDir.y;
  double tyMax = (farPlanes.y - r.origin.y) * invDir.y;

  if (tMin > tyMax || tyMin > tMax) return std::nullopt;
  if (tyMin > tMin) tMin = tyMin;
  if (tyMax < tMax) tMax = tyMax;

  double tzMin = (closePlanes.z - r.origin.z) * invDir.z;
  double tzMax = (farPlanes.z - r.origin.z) * invDir.z;

  if (tMin > tzMax || tzMin > tMax) return std::nullopt;
  if (tzMin > tMin) tMin = tzMin;
  if (tzMax < tMax) tMax = tzMax;

  DVec3 res;
  if (tMin <= 0) {
    if (tMax <= 0)
      return std::nullopt;
    else
      res = r.origin + tMax * r.dir;
  } else {
    res = r.origin + tMin * r.dir;
  }

  return res;
}

AaBox AaBox::GetAaBox() { return *this; }

void AaBox::Update(DVec3 point) {
  for (int i = 0; i < 3; i++) {
    bot_[i] = std::min(bot_[i], point[i]);
  }
  for (int i = 0; i < 3; i++) {
    top_[i] = std::max(top_[i], point[i]);
  }
}

void AaBox::Update(AaBox box) {
  Update(box.bot());
  Update(box.top());
}

bool AaBox::Inside(DVec3 point) const {
  return point.x < top_.x && point.x > bot_.x && point.y < top_.y &&
         point.y > bot_.y && point.z < top_.z && point.z > bot_.z;
}

double AaBox::SurfaceArea() const {
  double xdiff = top_.x - bot_.x;
  double ydiff = top_.y - bot_.y;
  double zdiff = top_.z - bot_.z;
  return 2 * xdiff * ydiff + 2 * ydiff * zdiff + 2 * xdiff * zdiff;
}

InterTri::InterTri(Material* material, DVertex vert0, DVertex vert1,
                   DVertex vert2)
    : material_(material) {
  verts_[0] = vert0;
  verts_[1] = vert1;
  verts_[2] = vert2;
}

std::optional<ShadeablePoint> InterTri::Intersect(const Ray& ray) {
  DVec3 verts[3] = {
      verts_[0].Position,
      verts_[1].Position,
      verts_[2].Position,
  };
  std::optional<DVec3> point = IntersectTri(ray, verts);
  if (!point.has_value()) {
    return std::nullopt;
  }
  return ShadeablePoint({*point, this, ray});
}

std::optional<DVec3> InterTri::EarliestIntersect(const Ray& ray) {
  std::optional<ShadeablePoint> intersect = Intersect(ray);
  if (intersect.has_value()) {
    return intersect->point;
  }
  return std::nullopt;
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
