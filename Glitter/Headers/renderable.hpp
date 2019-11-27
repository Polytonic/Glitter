#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "learnopengl/shader.h"

struct ShaderSet {
  Shader* texture_shader;
};

class Renderable {
public:
  virtual void Draw(ShaderSet shaders) = 0;
};

#endif
