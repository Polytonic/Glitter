#ifndef GLFW_MANAGER_H
#define GLFW_MANAGER_H

#include "glitter.hpp"
#include <GLFW/glfw3.h>

#include <string> 
#include <fstream> 
#include <sstream>
#include <iostream>

class GLFWManager
{
public:
    GLFWManager(int width, int height, const std::string& winName);
    GLFWwindow* GetWindow();
    void MakeCurrent();

    void SwapBuffers();
    bool ShouldClose();
    void SetShouldClose(bool close);
    bool WasKeyPressed(int key);
    void PollEvents();
    void TerminateAll();

private:
    // Define Some Constants
    int _width = 1280;
    int _height = 800;
    GLFWwindow* _window;
};

#endif //GLFW_MANAGER_H