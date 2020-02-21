#ifndef BOUND_HPP
#define BOUND_HPP

#include <memory>

#include "tracer/intersectable.hpp"

class BoundShape : public Intersectable {
 public:
  std::optional<ShadeablePoint> Intersect(const Ray& ray) override;
  virtual void AddChild(Intersectable* child) { children_.push_back(child); }

 protected:
  std::vector<Intersectable*> children_;
};

class BoundBox : public BoundShape {
 public:
  BoundBox();
  BoundBox(DVec3 bot, DVec3 top);
  void AddChild(Intersectable* child) override;
  std::optional<DVec3> EarliestIntersect(const Ray& ray) override;
  AaBox GetAaBox() override;
  bool IsShadeable() override { return false; }
  double SurfaceArea() const;

 protected:
  AaBox box_;
};

void AddChildren(BoundShape* shape, const std::vector<Intersectable*> children);

using BoundPtr = std::unique_ptr<BoundBox>;

#endif
