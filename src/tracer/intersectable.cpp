#include "tracer/intersectable.hpp"

#include <algorithm>
#include <iostream>

#include "shapes/interpolation.hpp"

std::optional<DVec3> IntersectTri(Ray ray, const std::array<DVec3, 3>& verts) {
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

double Intersectable::SurfaceArea() const { return GetAaBox().SurfaceArea(); }

AaBox::AaBox(DVec3 bot, DVec3 top) {
  initialized_ = true;
  bot_ = bot;
  top_ = top;
  Update(bot);
  Update(top);
}

AaBox::AaBox() {}

std::optional<ShadeablePoint> AaBox::Intersect(const Ray& ray) {
  return std::nullopt;
}

std::optional<DVec3> AaBox::EarliestIntersect(const Ray& ray) {
  if (!initialized_) {
    std::cerr << "ERROR: called `EarliestIntersect` on uninitialized AaBox."
              << std::endl;
    exit(-1);
  }

  // return EarliestIntersectSlowTriBased(ray);

  Ray r = ray;
  r.dir = glm::normalize(PreventZero(r.dir));
  DVec3 closePlanes(0);
  DVec3 farPlanes(0);
  DVec3 invDir(1.0 / r.dir.x, 1.0 / r.dir.y, 1.0 / r.dir.z);
  for (int i = 0; i < 3; i++) {
    if (r.dir[i] < 0) {
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

std::optional<DVec3> AaBox::EarliestIntersectSlowTriBased(const Ray& ray) {
  std::vector<std::array<DVec3, 3>> tris = ToTris();
  double closest = 1e100;
  std::optional<DVec3> closest_point;
  for (const auto& tri : tris) {
    std::optional<DVec3> inter = IntersectTri(ray, tri);
    if (inter.has_value()) {
      double curr = glm::distance(*inter, ray.origin);
      if (curr < closest) {
        closest = curr;
        closest_point = inter;
      }
    }
  }
  return closest_point;
}

AaBox AaBox::GetAaBox() const { return *this; }

DVec3 AaBox::EstimateCenter() const {
  if (!initialized_) {
    std::cerr << "ERROR: called `EstimateCenter` on uninitialized AaBox."
              << std::endl;
    exit(-1);
  }
  return (bot_ + top_) / 2.0;
}

void AaBox::Update(DVec3 point) {
  if (initialized_) {
    for (int i = 0; i < 3; i++) {
      bot_[i] = std::min(bot_[i], point[i]);
    }
    for (int i = 0; i < 3; i++) {
      top_[i] = std::max(top_[i], point[i]);
    }
  } else {
    bot_ = point;
    top_ = point;
  }
  initialized_ = true;
}

void AaBox::Update(AaBox box) {
  Update(box.bot());
  Update(box.top());
}

bool AaBox::Contains(DVec3 point) const {
  if (!initialized_) {
    std::cerr << "ERROR: called `Contains` on uninitialized AaBox."
              << std::endl;
    exit(-1);
  }
  return point.x <= top_.x && point.x >= bot_.x && point.y <= top_.y &&
         point.y >= bot_.y && point.z <= top_.z && point.z >= bot_.z;
}

bool AaBox::Contains(AaBox box) const {
  return Contains(box.bot()) && Contains(box.top());
}

bool AaBox::Contains(const Intersectable& inter) const {
  return Contains(inter.GetAaBox());
}

double AaBox::SurfaceArea() const {
  if (!initialized_) {
    std::cerr << "ERROR: called `SurfaceArea` on uninitialized AaBox."
              << std::endl;
    exit(-1);
  }
  double xdiff = top_.x - bot_.x;
  double ydiff = top_.y - bot_.y;
  double zdiff = top_.z - bot_.z;
  return 2 * xdiff * ydiff + 2 * ydiff * zdiff + 2 * xdiff * zdiff;
}

std::vector<std::array<DVec3, 3>> AaBox::ToTris() const {
  // Named with z going away, x going right, y up from behind origin (-z).
  DVec3 close_bot_left = bot_;
  DVec3 close_bot_right(top_.x, bot_.y, bot_.z);
  DVec3 close_top_right(top_.x, top_.y, bot_.z);
  DVec3 close_top_left(bot_.x, top_.y, bot_.z);
  DVec3 far_bot_left(bot_.x, bot_.y, top_.z);
  DVec3 far_bot_right(top_.x, bot_.y, top_.z);
  DVec3 far_top_right = top_;
  DVec3 far_top_left(bot_.x, top_.y, top_.z);

  std::vector<std::array<DVec3, 3>> tris;
  tris.reserve(12);

  // close face
  tris.push_back({close_bot_left, close_bot_right, close_top_right});
  tris.push_back({close_bot_left, close_top_right, close_top_left});

  // far face
  tris.push_back({far_bot_left, far_bot_right, far_top_right});
  tris.push_back({far_bot_left, far_top_right, far_top_left});

  // top face
  tris.push_back({close_top_left, close_top_right, far_top_right});
  tris.push_back({close_top_left, far_top_right, far_top_left});

  // bottom face
  tris.push_back({close_bot_left, close_bot_right, far_bot_right});
  tris.push_back({close_bot_left, far_bot_right, far_bot_left});

  // left face
  tris.push_back({close_bot_left, far_bot_left, far_top_left});
  tris.push_back({close_bot_left, far_top_left, close_top_left});

  // right face
  tris.push_back({close_bot_right, far_bot_right, far_top_right});
  tris.push_back({close_bot_right, far_top_right, close_top_right});

  return tris;
}

InterTri::InterTri(Material* material, Model* parent, DVertex vert0, DVertex vert1,
                   DVertex vert2)
    : material_(material), parent_(parent) {
  verts_[0] = vert0;
  verts_[1] = vert1;
  verts_[2] = vert2;
}

std::optional<ShadeablePoint> InterTri::Intersect(const Ray& ray) {
  std::optional<DVec3> point = IntersectTri(ray, {
                                                     verts_[0].Position,
                                                     verts_[1].Position,
                                                     verts_[2].Position,
                                                 });
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

AaBox InterTri::GetAaBox() const {
  AaBox box;
  for (int i = 0; i < 3; i++) {
    box.Update(verts_[i].Position);
  }
  return box;
}

DVec3 InterTri::EstimateCenter() const {
  return (verts_[0].Position + verts_[1].Position + verts_[2].Position) / 3.0;
}

Material* InterTri::material() const { return material_; }

DVec2 InterTri::GetUv(DVec3 point) {
  DVec3 weights = GetBarycentricWeights(verts_[0].Position, verts_[1].Position,
                                        verts_[2].Position, point);
  DVec2 uv(0.0);
  for (int i = 0; i < 3; i++) {
    uv += weights[i] * verts_[i].TexCoords;
  }
  return uv;
}

DVec3 InterTri::GetNormal(DVec3 point) {
  DVec3 weights = GetBarycentricWeights(verts_[0].Position, verts_[1].Position,
                                        verts_[2].Position, point);
  DVec3 normal(0.0);
  for (int i = 0; i < 3; i++) {
    normal += weights[i] * verts_[i].Normal;
  }
  return glm::normalize(normal);
}
