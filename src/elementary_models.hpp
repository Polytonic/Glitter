#ifndef ELEMENTARY_MODELS_HPP
#define ELEMENTARY_MODELS_HPP

#include <memory>

#include "learnopengl/mesh.h"
#include "learnopengl/model.h"

std::unique_ptr<Model> BuildBoxModel(Texture texture);

#endif
