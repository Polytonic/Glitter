#include "texture/texture_gen.hpp"

#include <vector>

#include "texture/tex_canvas.hpp"

namespace {

// Colors from
// https://sashat.me/2017/01/11/list-of-20-simple-distinct-colors/
// Sasha Trubetskoy
// 11 January 2017
// Accessed 03 December 2019

static const vector<RgbPix> kBasicColors = {
    {230, 25, 75},   {60, 180, 75},   {255, 225, 25}, {0, 130, 200},
    {245, 130, 48},  {145, 30, 180},  {70, 240, 240}, {240, 50, 230},
    {210, 245, 60},  {250, 190, 190}, {0, 128, 128},  {230, 190, 255},
    {170, 110, 40},  {255, 250, 200}, {128, 0, 0},    {170, 255, 195},
    {128, 128, 0},   {255, 215, 180}, {0, 0, 128},    {128, 128, 128},
    {255, 255, 255}, {0, 0, 0},
};

}  // namespace

Texture GetWhiteTexture(int width, int height) {
  TexCanvas canvas(width, height);
  return canvas.ToTexture();
}

Texture GetColorTexture(RgbPix color, int width, int height) {
  return GetColorCanvas(color, width, height).ToTexture();
}

TexCanvas GetColorCanvas(RgbPix color, int width, int height) {
  TexCanvas canvas(width, height);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      canvas.SetPix(i, j, color);
    }
  }
  return canvas;
}

void ApplyGrid(TexCanvas* canvas, int horizontal_lines, int vertical_lines,
               int pix_stroke, RgbPix color) {
  double x_step = ((double)canvas->width()) / horizontal_lines;
  for (int x_i = 0; x_i < horizontal_lines; x_i++) {
    for (int p = 0; p < pix_stroke; p++) {
      int x = (int)(x_step * x_i + p);
      for (int y = 0; y < canvas->height(); y++) {
        canvas->SetPix(x, y, color);
      }
    }
  }
  double y_step = ((double)canvas->height()) / vertical_lines;
  for (int y_i = 0; y_i < vertical_lines; y_i++) {
    for (int p = 0; p < pix_stroke; p++) {
      int y = (int)(y_step * y_i + p);
      for (int x = 0; x < canvas->width(); x++) {
        canvas->SetPix(x, y, color);
      }
    }
  }
}

RgbPix GetRandomBasicColor(std::default_random_engine* random_gen) {
  std::uniform_int_distribution<int> distribution(0, kBasicColors.size() - 1);
  return kBasicColors[distribution(*random_gen)];
}
