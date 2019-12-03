#ifndef RT_RENDER_UTIL_HPP
#define RT_RENDER_UTIL_HPP

#include <unordered_map>

#include "glitter.hpp"

#include "GLFW/glfw3.h"

void RenderCube();

bool KeyNewlyPressed(GLFWwindow* window, std::unordered_map<int, bool>* key_states, int key);

#endif
