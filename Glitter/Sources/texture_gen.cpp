#include "texture_gen.hpp"

#include "tex_canvas.hpp"

Texture GetWhiteTexture(int width, int height) {
  TexCanvas canvas(width, height);
  return canvas.ToTexture();
}
