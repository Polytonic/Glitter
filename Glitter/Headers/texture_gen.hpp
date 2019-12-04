#ifndef TEXTURE_GEN_HPP
#define TEXTURE_GEN_HPP

#include <random>

#include "glitter.hpp"
#include "learnopengl/mesh.h"
#include "tex_canvas.hpp"

Texture GetWhiteTexture(int width = 100, int height = 100);
Texture GetColorTexture(RgbPix color, int width = 100, int height = 100);
RgbPix GetRandomBasicColor(std::default_random_engine* random_gen);

#endif
