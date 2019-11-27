// Preprocessor Directives
#ifndef GLITTER
#define GLITTER
#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

// System Headers
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Define Some Constants
const int mWidth = 1280;
const int mHeight = 800;

typedef glm::dvec2 DVec2;
typedef glm::dvec3 DVec3;
typedef glm::dvec4 DVec4;

typedef glm::dmat2 DMat2;
typedef glm::dmat3 DMat3;
typedef glm::dmat4 DMat4;

#endif  //~ Glitter Header
