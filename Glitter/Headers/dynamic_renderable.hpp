#ifndef DYNAMIC_RENDERABLE_HPP
#define DYNAMIC_RENDERABLE_HPP

#include "renderable.hpp"

class DynamicRenderable : public Renderable {
public:
  virtual void Tick(double delta_sec) = 0;
  virtual void Draw(ShaderSet shaders) = 0;
  virtual glm::mat4 GetModelMatrix() = 0;
};

#endif
