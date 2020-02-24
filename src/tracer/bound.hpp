#ifndef BOUND_HPP
#define BOUND_HPP

#include <memory>
#include <vector>

#include "tracer/intersectable.hpp"

class BoundShape;
using BoundPtr = std::unique_ptr<BoundShape>;

class BoundShape : public Intersectable {
 public:
  std::optional<ShadeablePoint> Intersect(const Ray& ray) override;
  virtual void AddChild(Intersectable* child) {
    inter_children_.push_back(child);
  }
  virtual void AddChild(BoundPtr child) {
    bound_children_.push_back(std::move(child));
  }
  virtual bool empty() const {
    return inter_children_.empty() && bound_children_.empty();
  }
  virtual size_t size() const {
    return inter_children_.size() + bound_children_.size();
  }
  virtual const std::vector<Intersectable*>& inter_children() {
    return inter_children_;
  }
  virtual std::vector<BoundPtr>& bound_children() { return bound_children_; }
  virtual size_t RecursiveShadeableSize() const;
  virtual void RecursiveAssertSanity() const;

 protected:
  std::vector<Intersectable*> inter_children_;
  std::vector<BoundPtr> bound_children_;
};

class BoundBox : public BoundShape {
 public:
  BoundBox();
  BoundBox(DVec3 bot, DVec3 top);
  void AddChild(Intersectable* child) override;
  void AddChild(BoundPtr child) override;
  std::optional<DVec3> EarliestIntersect(const Ray& ray) override;
  AaBox GetAaBox() const override;
  DVec3 EstimateCenter() const override;
  bool IsShadeable() const override { return false; }
  double SurfaceArea() const override;

 protected:
  AaBox box_;
};

void AddChildren(BoundShape* shape, const std::vector<Intersectable*> children);

#endif
