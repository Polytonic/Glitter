#ifndef DYNAMIC_RENDERABLE_HPP
#define DYNAMIC_RENDERABLE_HPP

#include "renderable.hpp"

class DynamicRenderable : public Renderable {
public:
  virtual void Tick(double delta_sec) = 0;
  virtual void Draw(ShaderSet shaders, glm::mat4 model_mat) = 0;
};

#endif
