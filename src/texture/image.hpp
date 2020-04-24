#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

#include "scene/primitives.hpp"

void BuildGlTexture(Texture* texture);
Texture TextureFromFile(const std::string& filename,
                        const std::string& typeName, bool gamma = false);
Texture TextureFromFile(const std::string& path, const std::string& directory,
                        const std::string& typeName, bool gamma = false);

void TextureToFile(const std::string& filename, const Texture& tex);

#endif
