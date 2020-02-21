#include "tex_canvas.hpp"

#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "learnopengl/model.h"

TexCanvas::TexCanvas(int width, int height, int channels)
    : width_(width), height_(height), channels_(channels) {
  GLint row_alignment_div;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &row_alignment_div);
  row_alignment_ = (width_ * channels_) + (width_ % row_alignment_div);
  int mem_size = sizeof(unsigned char) * row_alignment_ * height_;
  data = (unsigned char*)malloc(mem_size);
  // Set everything to white
  memset(data, 255, mem_size);
}

bool TexCanvas::SetPix(int x, int y, RgbPix pix) {
  if (channels_ != 3) {
    std::cerr << "Attempting to set RGB on canvas with " << channels_
              << " channels.";
    exit(-1);
  }
  if (x >= width_ || y >= height_ || x < 0 || y < 0) {
    return false;
  }
  unsigned char* pixel = data + (row_alignment_ * y) + (x * channels_);
  *pixel = pix.r;
  *(pixel + 1) = pix.g;
  *(pixel + 2) = pix.b;
  return true;
}

bool TexCanvas::SetPix(int x, int y, RgbPix pix, unsigned char a) {
  if (channels_ != 4) {
    std::cerr << "Attempting to set RGBA on canvas with " << channels_
              << " channels.";
    exit(-1);
  }
  if (x >= width_ || y >= height_ || x < 0 || y < 0) {
    return false;
  }
  unsigned char* pixel = data + (row_alignment_ * y) + (x * channels_);
  *pixel = pix.r;
  *(pixel + 1) = pix.g;
  *(pixel + 2) = pix.b;
  *(pixel + 3) = a;
  return true;
}

bool TexCanvas::SetPix(int x, int y, unsigned char val) {
  if (channels_ != 1) {
    std::cerr << "Attempting to set gray on canvas with " << channels_
              << " channels.";
    exit(-1);
  }
  if (x >= width_ || y >= height_ || x < 0 || y < 0) {
    return false;
  }
  unsigned char* pixel = data + (row_alignment_ * y) + (x * channels_);
  *pixel = val;
  return true;
}

Texture TexCanvas::ToTexture(std::string texture_type) {
  Texture tex;
  tex.type = std::move(texture_type);
  tex.path = "generated";
  tex.width = width_;
  tex.height = height_;
  tex.num_components = channels_;
  tex.row_alignment = row_alignment_;
  tex.data = data;
  BuildGlTexture(&tex);
  return tex;
}
