cmake_minimum_required(VERSION 3.0)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(bullet)
# Bullet does not export includes, so we do it manually
target_include_directories(BulletDynamics INTERFACE bullet/src/)

add_subdirectory(glm)

# STB does not use CMake, so we create a minimal header-only target
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb/)

# Glad does not use CMake, so we create a minimal target
add_library(glad glad/src/glad.c)
if(NOT WIN32)
    target_link_libraries(glad PUBLIC dl)
endif()
target_include_directories(glad PUBLIC glad/include/)
