#ifndef REALTIME_RT_RENDER_UTIL_HPP
#define REALTIME_RT_RENDER_UTIL_HPP

#include <unordered_map>

#include "learnopengl/glitter.hpp"

#include "GLFW/glfw3.h"

void RenderCube();

bool KeyNewlyPressed(GLFWwindow* window,
                     std::unordered_map<int, bool>* key_states, int key);

#endif
