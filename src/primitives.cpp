#include "primitives.hpp"

DVertex::DVertex(const Vertex& v)
    : Position(v.Position),
      Normal(v.Normal),
      TexCoords(v.TexCoords),
      Bitangent(v.Bitangent) {}

Material::Material(Texture diff_texture) : diff_texture_(diff_texture) {}

Material::Material(Texture diff_texture, Transparency transparency)
    : diff_texture_(diff_texture), transparency_(transparency) {}

void DVertex::Apply(DMat4 mat) {
  Position = mat * DVec4(Position, 1.0);
  Normal = glm::transpose(glm::inverse(DMat3(mat))) * Normal;
}

RgbPix Texture::Sample(double u, double v) const {
  int x = std::round(u * width);
  x = std::max(x, 0);
  x = std::min(x, width - 1);
  int y = std::round(v * width);
  y = std::max(y, 0);
  y = std::min(y, width - 1);
  unsigned char* pixel = data + (row_alignment * y) + (x * num_components);
  if (num_components < 3) {
    return RgbPix({
        *pixel,
        *pixel,
        *pixel,
    });
  } else {
    return RgbPix({
        *pixel,
        *(pixel + 1),
        *(pixel + 2),
    });
  }
}

RgbPix Texture::Sample(DVec2 uv) const { return Sample(uv.x, uv.y); }
