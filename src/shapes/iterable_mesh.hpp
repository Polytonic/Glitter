#ifndef SHAPES_ITERABLE_MESH_HPP
#define SHAPES_ITERABLE_MESH_HPP

#include "learnopengl/glitter.hpp"

struct ComputedVertex {
  DVec3 position = DVec3(0);
  DVec3 normal = DVec3(0);
};

class IterableMesh {
 public:
  virtual ~IterableMesh() = default;
  virtual ComputedVertex GetVertex(double u, double v) = 0;
  virtual bool IsClosed() = 0;
};

class IterableRectPlane : public IterableMesh {
 public:
  IterableRectPlane(double length, double width);

  ComputedVertex GetVertex(double u, double v) override;

  bool IsClosed() override { return false; }

 private:
  double length_;
  double width_;
};

class IterableCylinder : public IterableMesh {
 public:
  IterableCylinder(double height, double radius);

  ComputedVertex GetVertex(double u, double v) override;

  bool IsClosed() override { return false; }

 private:
  double height_;
  double radius_;
};

class IterableSphere : public IterableMesh {
 public:
  IterableSphere(double radius);

  ComputedVertex GetVertex(double u, double v) override;

  bool IsClosed() override { return false; }

 private:
  double radius_;
};

// Will have 2 * loops_per_unit loops.
class IterableHelix : public IterableMesh {
 public:
  IterableHelix(double helix_radius, double helix_height, double fiber_radius,
                double loops_per_unit);

  ComputedVertex GetVertex(double u, double v) override;

  bool IsClosed() override { return false; }

 private:
  double helix_radius_;
  double helix_height_;
  double fiber_radius_;
  double loops_per_unit_;
};

#endif
