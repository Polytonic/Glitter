#include "learnopengl/mesh.h"

DVertex::DVertex(const Vertex& v) :
  Position(v.Position),
  Normal(v.Normal),
  TexCoords(v.TexCoords),
  Bitangent(v.Bitangent) {}

Material::Material(Texture diff_texture) :
  diff_texture_(diff_texture) {}

Material::Material(Texture diff_texture, Transparency transparency) :
  diff_texture_(diff_texture), transparency_(transparency) {}
