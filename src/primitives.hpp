#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <string>

#include "glitter.hpp"

struct RgbPix {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};

struct DVertex {
  DVertex() = default;
  DVertex(const Vertex& v);
  // position
  DVec3 Position;
  // normal
  DVec3 Normal;
  // texCoords
  DVec2 TexCoords;
  // tangent
  DVec3 Tangent;
  // bitangent
  DVec3 Bitangent;

  void Apply(DMat4 mat);
};

struct Light {
  glm::vec3 Position;
  glm::vec3 Color;

  float Linear = 0.2f;
  float Quadratic = 0.4f;
};

struct Texture {
  unsigned int id = 0;
  std::string type;
  std::string path;
  int width = 0;
  int height = 0;
  int num_components = 0;
  int row_alignment = 0;
  unsigned char* data = nullptr;

  RgbPix Sample(double u, double v) const;
  RgbPix Sample(DVec2 uv) const;
};

struct Transparency {
  double opacity = 1.0;
  double index = 1.0003;
};

class Material {
 public:
  Material(Texture diff_texture);
  Material(Texture diff_texture, Transparency transparency);

  const Texture& diff_texture() const { return diff_texture_; }
  const Transparency& transparency() const { return transparency_; }
  double opacity() const { return transparency_.opacity; }
  double r_index() const { return transparency_.index; }

 private:
  Texture diff_texture_;
  Transparency transparency_;
};

#endif
