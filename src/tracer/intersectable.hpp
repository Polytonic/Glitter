#ifndef TRACER_INTERSECTABLE_HPP
#define TRACER_INTERSECTABLE_HPP

#include <memory>
#include <optional>
#include <vector>

#include "glitter.hpp"
#include "primitives.hpp"

class AaBox;
class Intersectable;
class Shadeable;

DVec3 PreventZero(DVec3 vec);

struct Ray {
  DVec3 origin;
  DVec3 dir;
};

struct InterPoint {
  DVec3 point;
  Intersectable* shape;
  Ray ray;
};

struct ShadeablePoint {
  DVec3 point;
  Shadeable* shape;
  Ray ray;
};

std::optional<DVec3> IntersectTri(Ray ray, DVec3 verts[3]);

class Intersectable {
 public:
  virtual std::optional<ShadeablePoint> Intersect(const Ray& ray) = 0;
  virtual std::optional<DVec3> EarliestIntersect(const Ray& ray) = 0;
  virtual AaBox GetAaBox() const = 0;
  virtual double SurfaceArea() const;
  virtual DVec3 EstimateCenter() const = 0;
  virtual bool IsShadeable() const = 0;
};

using InterPtr = std::unique_ptr<Intersectable>;

class Shadeable : public Intersectable {
 public:
  virtual Material* material() const = 0;
  virtual DVec2 GetUv(DVec3 point) = 0;
};

class AaBox : public Intersectable {
 public:
  AaBox(DVec3 bot, DVec3 top);
  AaBox(DVec3 point);
  AaBox();
  std::optional<ShadeablePoint> Intersect(const Ray& ray) override;
  std::optional<DVec3> EarliestIntersect(const Ray& ray) override;
  AaBox GetAaBox() const override;
  double SurfaceArea() const override;
  DVec3 EstimateCenter() const override;
  bool IsShadeable() const override { return false; }
  virtual void Update(DVec3 point);
  virtual void Update(AaBox box);
  virtual bool Contains(DVec3 point) const;
  virtual bool Contains(AaBox box) const;
  virtual bool Contains(const Intersectable& inter) const;

  DVec3 bot() { return bot_; }
  DVec3 top() { return top_; }

 protected:
  DVec3 bot_;
  DVec3 top_;
};

class InterTri : public Shadeable {
 public:
  InterTri(Material* material, DVertex vert0, DVertex vert1, DVertex vert2);
  std::optional<ShadeablePoint> Intersect(const Ray& ray) override;
  std::optional<DVec3> EarliestIntersect(const Ray& ray) override;
  AaBox GetAaBox() const override;
  DVec3 EstimateCenter() const override;
  bool IsShadeable() const override { return true; }
  Material* material() const override;
  DVec2 GetUv(DVec3 point) override;

 protected:
  Material* material_;
  DVertex verts_[3];
};

#endif
