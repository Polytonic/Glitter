#include "primitives.hpp"

DVertex::DVertex(const Vertex& v)
    : Position(v.Position),
      Normal(v.Normal),
      TexCoords(v.TexCoords),
      Bitangent(v.Bitangent) {}

Material::Material(Texture diff_texture) : diff_texture_(diff_texture) {}

Material::Material(Texture diff_texture, Transparency transparency)
    : diff_texture_(diff_texture), transparency_(transparency) {}

void DVertex::Apply(DMat4 mat){
  Position = mat * DVec4(Position, 1.0);
  Normal = glm::transpose(glm::inverse(DMat3(mat))) * Normal;
}
