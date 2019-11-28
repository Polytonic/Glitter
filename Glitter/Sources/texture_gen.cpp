#include "texture_gen.hpp"

#include "tex_canvas.hpp"

Texture GetWhiteTexture(int width, int height) {
  TexCanvas canvas(width, height);
  return canvas.ToTexture();
}

Texture GetColorTexture(TexCanvas::RgbPix color, int width, int height){
  TexCanvas canvas(width, height);
  for(int i = 0; i < width; i++){
    for(int j = 0; j < width; j++){
      canvas.SetPix(i, j, color);
    }
  }
  return canvas.ToTexture();
}
