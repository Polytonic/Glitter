#define _USE_MATH_DEFINES
#include <cmath>

#include "iterable_mesh.hpp"

#include <iostream>

#include "glm/gtx/transform.hpp"

IterableRectPlane::IterableRectPlane(double length, double width)
    : length_(length), width_(width) {}

ComputedVertex IterableRectPlane::GetVertex(double u, double v) {
  ComputedVertex vertex;
  vertex.position.x = (u * length_) - (length_ / 2.0);
  vertex.position.y = 0;
  vertex.position.z = (v * width_) - (width_ / 2.0);
  vertex.normal.x = 0;
  vertex.normal.y = 1;
  vertex.normal.z = 0;
  return vertex;
}

IterableCylinder::IterableCylinder(double height, double radius)
    : height_(height), radius_(radius) {}

ComputedVertex IterableCylinder::GetVertex(double u, double v) {
  ComputedVertex vertex;
  vertex.normal.x = std::sin(2.0 * M_PI * u);
  vertex.normal.z = std::cos(2.0 * M_PI * u);
  vertex.normal.y = 0;

  vertex.position.x = vertex.normal.x * radius_;
  vertex.position.z = vertex.normal.z * radius_;
  vertex.position.y = v * height_ - (height_ / 2.0);
  return vertex;
}

IterableSphere::IterableSphere(double radius) : radius_(radius) {}

ComputedVertex IterableSphere::GetVertex(double u, double v) {
  if (u > 1.0 || u < 0.0) {
    std::cerr << "Got u " << u << std::endl;
  }
  if (v > 1.0 || v < 0.0) {
    std::cerr << "Got v " << v << std::endl;
  }
  ComputedVertex vertex;
  double theta = u * 2.0 * M_PI;
  double phi = v * M_PI;
  vertex.normal.x = std::sin(theta) * std::sin(phi);
  vertex.normal.y = std::cos(phi);
  vertex.normal.z = std::cos(theta) * std::sin(phi);

  vertex.position.x = vertex.normal.x * radius_;
  vertex.position.y = vertex.normal.y * radius_;
  vertex.position.z = vertex.normal.z * radius_;
  return vertex;
}

IterableHelix::IterableHelix(double helix_radius, double helix_height,
                             double fiber_radius, double loops_per_unit)
    : helix_radius_(helix_radius),
      helix_height_(helix_height),
      fiber_radius_(fiber_radius),
      loops_per_unit_(loops_per_unit) {}

ComputedVertex IterableHelix::GetVertex(double u, double v) {
  ComputedVertex vertex;
  v = 2 * v - 1.0;
  u = 1.0 - u;
  double current_radians = v * helix_height_ * loops_per_unit_ * 2 * M_PI;

  double slope = 1.0 / (helix_radius_ * 2 * M_PI * loops_per_unit_);

  DVec4 fiber_position_flat(std::sin(current_radians), 0,
                            std::cos(current_radians), 0);
  DVec3 fiber_position(helix_radius_ * fiber_position_flat.x, v * helix_height_,
                       helix_radius_ * fiber_position_flat.z);
  // Rotate flat fiber position 90 degress about the vertical axis.
  DVec4 horizontal_dir =
      glm::rotate((double)M_PI / 2.0, DVec3(0, 1, 0)) * fiber_position_flat;
  // Since horizontal_dir is normalized, just stick in slope
  // for y.
  DVec3 norm_fiber_dir(horizontal_dir.x, slope, horizontal_dir.z);
  norm_fiber_dir = glm::normalize(norm_fiber_dir);
  DVec3 norm_xz_fiber_pos(fiber_position_flat.x, 0, fiber_position_flat.z);
  DVec3 unrotated_surface_pos = norm_xz_fiber_pos * fiber_radius_;
  DMat4 rot_mat = glm::rotate(u * 2 * (double)M_PI, norm_fiber_dir);
  DVec4 surface_pos4 = rot_mat * DVec4(unrotated_surface_pos, 1.0);
  if (surface_pos4[3] != 1.0) {
    std::cerr << "got 4th element " << surface_pos4[3] << std::endl;
    exit(-1);
  }
  DVec3 surface_pos = DVec3(surface_pos4.x, surface_pos4.y, surface_pos4.z);

  vertex.normal = glm::normalize(surface_pos);
  vertex.position = fiber_position + surface_pos;
  return vertex;
}
