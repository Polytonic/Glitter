#include <learnopengl/model.h>

void BuildGlTexture(Texture* texture) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  texture->id = textureID;
  GLenum format;
  switch (texture->num_components) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      std::cerr << "Unexpected number of components " << texture->num_components
                << " in texture from " << texture->path << std::endl;
      exit(-1);
      break;
  }

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0,
               format, GL_UNSIGNED_BYTE, texture->data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture TextureFromFile(const char* path, const string& directory,
                        const string& typeName, bool gamma) {
  const char kPathSeparator =
#ifdef _WIN32
      '\\';
#else
      '/';
#endif
  string filename = string(path);
  filename = directory + kPathSeparator + filename;
#ifdef _WIN32
  std::replace(filename.begin(), filename.end(), '/', '\\');
#else
  std::replace(filename.begin(), filename.end(), '\\', '/');
#endif

  Texture texture;
  texture.type = typeName;
  texture.path = path;

  unsigned char* data = stbi_load(filename.c_str(), &texture.width,
                                  &texture.height, &texture.num_components, 0);
  if (data) {
    texture.data = data;
    BuildGlTexture(&texture);
  } else {
    std::cerr << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
    exit(-1);
  }

  return texture;
}
