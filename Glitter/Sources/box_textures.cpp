#include "box_textures.hpp"

#include "tex_canvas.hpp"

Texture GetTestBoxTexture(std::default_random_engine* random_gen) {
  const int width = 1000;
  const int height = 1000;
  TexCanvas canvas(width, height);
  for (int x = 0; x < width; x++) {
    unsigned char red = (unsigned char)((x / (float)width) * 255);
    unsigned char green = (unsigned char)(((width - x) / (float)width) * 255);
    for (int y = 0; y < height; y++) {
      unsigned char blue = (unsigned char)((y / (float)height) * 255);
      canvas.SetPix(x, y, {red, green, blue});
    }
  }
  return canvas.ToTexture();
}
