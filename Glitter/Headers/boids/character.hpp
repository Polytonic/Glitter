#ifndef BOIDS_CHARACTER_HPP
#define BOIDS_CHARACTER_HPP

#include <memory>
#include <random>

#include "box_textures.hpp"
#include "iterable_mesh.hpp"
#include "learnopengl/mesh.h"
#include "learnopengl/model.h"
#include "mesh_iterator.hpp"

std::unique_ptr<Model> GetBoidCharacter(std::default_random_engine* random_gen);

#endif
