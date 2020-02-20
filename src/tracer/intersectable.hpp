#ifndef TRACER_INTERSECTABLE_HPP
#define TRACER_INTERSECTABLE_HPP

#include <optional>

#include "glitter.hpp"
#include "learnopengl/mesh.h"

class AaBox;
class Intersectable;

struct InterPoint {
  DVec3 point;
  Intersectable* intersectable;
};

struct Ray {
  DVec3 origin;
  DVec3 dir;
};

std::optional<DVec3> IntersectTri(const Ray& ray, DVec3 verts[3]);

class Intersectable {
 public:
  virtual std::optional<InterPoint> Intersect(const Ray& ray) = 0;
  virtual AaBox GetAaBox() = 0;
};

class Shadeable {
 public:
  virtual Material* material() const = 0;
  virtual DVec2 GetUv(DVec3 point) = 0;
};

class AaBox : public Intersectable {
 public:
  DVec3 bot;
  DVec3 top;
  AaBox(DVec3 bot, DVec3 top);
  AaBox(DVec3 point);
  std::optional<InterPoint> Intersect(const Ray& ray) override;
  AaBox GetAaBox() override;
  void Update(DVec3 point);
};

class InterTri : public Intersectable, public Shadeable {
 public:
  InterTri(Material* material, DVertex vert0, DVertex vert1, DVertex vert2);
  std::optional<InterPoint> Intersect(const Ray& ray) override;
  AaBox GetAaBox() override;
  Material* material() const override;
  DVec2 GetUv(DVec3 point) override;

 private:
  Material* material_;
  DVertex verts_[3];
};

#endif
