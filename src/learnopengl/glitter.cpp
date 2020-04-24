#include "learnopengl/glitter.hpp"

std::ostream& operator<<(std::ostream& os, const DVec3 vec) {
  os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const DVec2 vec) {
  os << "(" << vec.x << ", " << vec.y << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::optional<DVec3> vec) {
  if (vec.has_value()) {
    os << *vec;
  } else {
    os << "(no vector)";
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3 vec) {
  os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return os;
}

double epsilon(DVec3 vec) { return 1e-10; }
