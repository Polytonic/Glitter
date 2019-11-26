#ifndef ITERABLE_MESH_HPP
#define ITERABLE_MESH_HPP

#include "glitter.hpp"

struct ComputedVertex {
  glm::vec3 position = glm::vec3(0);
  glm::vec3 normal = glm::vec3(0);
};

class IterableMesh {
 public:
  virtual ComputedVertex GetVertex(float u, float v) = 0;
  virtual bool IsClosed() = 0;
};

class IterableRectPlane : public IterableMesh {
 public:
  IterableRectPlane(float length, float width);

  ComputedVertex GetVertex(float u, float v) override;

  bool IsClosed() override { return false; }

 private:
  float length_;
  float width_;
};

class IterableCylinder : public IterableMesh {
 public:
  IterableCylinder(float height, float radius);

  ComputedVertex GetVertex(float u, float v) override;

  bool IsClosed() override { return true; }

 private:
  float height_;
  float radius_;
};

class IterableSphere : public IterableMesh {
 public:
  IterableSphere(float radius);

  ComputedVertex GetVertex(float u, float v) override;

  bool IsClosed() override { return true; }

 private:
  float radius_;
};

// Will have 2 * loops_per_unit loops.
class IterableHelix : public IterableMesh {
 public:
  IterableHelix(float helix_radius, float helix_height, float fiber_radius,
                float loops_per_unit);

  ComputedVertex GetVertex(float u, float v) override;

  bool IsClosed() override { return true; }

 private:
  float helix_radius_;
  float helix_height_;
  float fiber_radius_;
  float loops_per_unit_;
};

#endif
