// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


namespace utils{

	enum class fileType{HEADER, SOURCE, TEXTURE, SHADER };

	std::string getPath(const std::string& name, const fileType& type);

	float* getCubePoints(size_t& size);
}