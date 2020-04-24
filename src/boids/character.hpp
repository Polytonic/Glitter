#ifndef BOIDS_CHARACTER_HPP
#define BOIDS_CHARACTER_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#include <memory>
#include <random>

#include "texture/box_textures.hpp"
#include "learnopengl/glitter.hpp"
#include "shapes/iterable_mesh.hpp"
#include "learnopengl/mesh.h"
#include "learnopengl/model.h"
#include "shapes/mesh_iterator.hpp"

std::unique_ptr<Model> GetBoidCharacter(std::default_random_engine* random_gen,
                                        RgbPix color1, RgbPix color2,
                                        RgbPix color3);

#endif
