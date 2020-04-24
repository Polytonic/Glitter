#ifndef TEXTURE_TEX_CANVAS_HPP
#define TEXTURE_TEX_CANVAS_HPP

#include "learnopengl/glitter.hpp"

#include "learnopengl/mesh.h"

class TexCanvas {
 public:
  TexCanvas(int width, int height, int channels = 3);

  // Return false iff x, y is outside the image size.
  //
  // Check-fail if the number of channels does not match
  // the third argument.
  bool SetPix(int x, int y, RgbPix pix);
  bool SetPix(int x, int y, RgbPix pix, unsigned char a);
  bool SetPix(int x, int y, unsigned char val);

  Texture ToTexture(std::string texture_type = "texture_diffuse");

  int width() const { return width_; }
  int height() const { return height_; }
  int channels() const { return channels_; }

 private:
  int width_;
  int height_;
  int row_alignment_;
  int channels_;
  unsigned char* data;
};

#endif
