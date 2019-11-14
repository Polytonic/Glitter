#ifndef BOX_BUILDER_HPP
#define BOX_BUILDER_HPP

#include <memory>

#include "learnopengl/mesh.h"
#include "learnopengl/model.h"

std::unique_ptr<Model> BuildBoxModel(Texture texture);

#endif
