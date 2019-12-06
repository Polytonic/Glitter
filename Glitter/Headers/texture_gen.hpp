#ifndef TEXTURE_GEN_HPP
#define TEXTURE_GEN_HPP

#include "glitter.hpp"

#include <random>

#include "learnopengl/mesh.h"
#include "tex_canvas.hpp"

Texture GetWhiteTexture(int width = 100, int height = 100);
Texture GetColorTexture(RgbPix color, int width = 100, int height = 100);
TexCanvas GetColorCanvas(RgbPix color, int width = 100, int height = 100);
void ApplyGrid(TexCanvas* canvas, int horizontal_lines, int vertical_lines,
               int pix_stroke, RgbPix color);
RgbPix GetRandomBasicColor(std::default_random_engine* random_gen);

#endif
