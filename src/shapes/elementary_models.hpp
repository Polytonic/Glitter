#ifndef SHAPES_ELEMENTARY_MODELS_HPP
#define SHAPES_ELEMENTARY_MODELS_HPP

#include <memory>

#include "learnopengl/mesh.h"
#include "learnopengl/model.h"

std::unique_ptr<Model> BuildBoxModel(Material material);

#endif
