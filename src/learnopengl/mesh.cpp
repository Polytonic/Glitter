#include "learnopengl/mesh.h"

Material::Material(Texture diff_texture) :
  diff_texture_(diff_texture) {}

Material::Material(Texture diff_texture, Transparency transparency) :
  diff_texture_(diff_texture), transparency_(transparency) {}
