#ifndef TEXTURE_GEN_HPP
#define TEXTURE_GEN_HPP

#include <random>

#include "learnopengl/mesh.h"
#include "tex_canvas.hpp"

Texture GetWhiteTexture(int width = 100, int height = 100);
Texture GetColorTexture(TexCanvas::RgbPix color, int width = 100, int height = 100);

#endif
