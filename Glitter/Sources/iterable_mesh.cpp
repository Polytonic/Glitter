#include "iterable_mesh.hpp"

#include <math.h>
#include <iostream>

#include "glm/gtx/transform.hpp"

IterableCylinder::IterableCylinder(float height, float radius)
    : height_(height), radius_(radius) {}

ComputedVertex IterableCylinder::GetVertex(float u, float v) {
  ComputedVertex vertex;
  vertex.normal.x = std::sin(2.0f * M_PI * u);
  vertex.normal.z = std::cos(2.0f * M_PI * u);
  vertex.normal.y = 0;

  vertex.position.x = vertex.normal.x * radius_;
  vertex.position.z = vertex.normal.z * radius_;
  vertex.position.y = v * height_ - (height_ / 2.0f);
  return vertex;
}

IterableSphere::IterableSphere(float radius) : radius_(radius) {}

ComputedVertex IterableSphere::GetVertex(float u, float v) {
  ComputedVertex vertex;
  float theta = u * 2.0f * M_PI;
  float phi = v * M_PI;
  vertex.normal.x = std::sin(theta) * std::sin(phi);
  vertex.normal.y = std::cos(phi);
  vertex.normal.z = std::cos(theta) * std::sin(phi);

  vertex.position.x = vertex.normal.x * radius_;
  vertex.position.y = vertex.normal.y * radius_;
  vertex.position.z = vertex.normal.z * radius_;
  return vertex;
}

IterableHelix::IterableHelix(float helix_radius, float helix_height,
                             float fiber_radius, float loops_per_unit)
    : helix_radius_(helix_radius),
      helix_height_(helix_height),
      fiber_radius_(fiber_radius),
      loops_per_unit_(loops_per_unit) {}

ComputedVertex IterableHelix::GetVertex(float u, float v) {
  ComputedVertex vertex;
  v = 2 * v - 1.0f;
  float current_radians = v * helix_height_ * loops_per_unit_ * 2 * M_PI;

  float slope = 1.0f / (helix_radius_ * 2 * M_PI * loops_per_unit_);

  glm::vec4 fiber_position_flat(std::sin(current_radians), 0,
                                std::cos(current_radians), 0);
  glm::vec3 fiber_position(helix_radius_ * fiber_position_flat.x,
                           v * helix_height_,
                           helix_radius_ * fiber_position_flat.z);
  // Rotate flat fiber position 90 degress about the vertical axis.
  glm::vec4 horizontal_dir =
      glm::rotate((float)M_PI / 2.0f, glm::vec3(0, 1, 0)) * fiber_position_flat;
  // Since horizontal_dir is normalized, just stick in slope
  // for y.
  glm::vec3 norm_fiber_dir(horizontal_dir.x, slope, horizontal_dir.z);
  norm_fiber_dir = glm::normalize(norm_fiber_dir);
  glm::vec3 norm_xz_fiber_pos(fiber_position_flat.x, 0, fiber_position_flat.z);
  glm::vec3 unrotated_surface_pos = norm_xz_fiber_pos * fiber_radius_;
  glm::mat4 rot_mat = glm::rotate(u * 2 * (float)M_PI, norm_fiber_dir);
  glm::vec4 surface_pos4 = rot_mat * glm::vec4(unrotated_surface_pos, 1.0f);
  if (surface_pos4[3] != 1.0f) {
    std::cerr << "got 4th element " << surface_pos4[3] << std::endl;
    exit(-1);
  }
  glm::vec3 surface_pos =
      glm::vec3(surface_pos4.x, surface_pos4.y, surface_pos4.z);

  vertex.normal = glm::normalize(surface_pos);
  vertex.position = fiber_position + surface_pos;
  return vertex;
}
