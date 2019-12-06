#ifndef BOIDS_CHARACTER_HPP
#define BOIDS_CHARACTER_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#include <memory>
#include <random>

#include "box_textures.hpp"
#include "glitter.hpp"
#include "iterable_mesh.hpp"
#include "learnopengl/mesh.h"
#include "learnopengl/model.h"
#include "mesh_iterator.hpp"

std::unique_ptr<Model> GetBoidCharacter(std::default_random_engine* random_gen,
                                        RgbPix color1, RgbPix color2,
                                        RgbPix color3);

#endif
