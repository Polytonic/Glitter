// Local Headers
#include "GLFWManager.h"

GLFWManager::GLFWManager(int width, int height, const std::string& winName)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	_window = glfwCreateWindow(width, height, winName.c_str(), NULL, NULL);
	if (_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(_window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return;
	}
}

GLFWwindow* GLFWManager::GetWindow() {
	return _window;
}

void GLFWManager::MakeCurrent() {
	glfwMakeContextCurrent(_window);
}

void GLFWManager::SwapBuffers() {
	glfwSwapBuffers(_window);
}

bool GLFWManager::ShouldClose() {
	return glfwWindowShouldClose(_window);
}

void GLFWManager::SetShouldClose(bool close) {
	glfwSetWindowShouldClose(_window, close);
}

bool GLFWManager::WasKeyPressed(int key) {
	return glfwGetKey(_window, key) == GLFW_PRESS;
}

void GLFWManager::PollEvents(){
	glfwPollEvents();
}

void GLFWManager::TerminateAll() {
	glfwTerminate();
}