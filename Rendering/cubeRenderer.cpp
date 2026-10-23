#include "cubeRenderer.hpp"

CubeRenderer::CubeRenderer() {
	window_ = openGLinit();
	meshGenerator_.createCube(cubeVAO_, cubeVBO_);
	meshGenerator_.createPlane(planeVAO_, planeVBO_);
}

GLFWwindow* CubeRenderer::openGLinit() {

  if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW\n");
	}

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Cube Multiplayer Game", NULL, NULL);
  glfwMakeContextCurrent(window); 

  glfwSwapInterval(1);
	
	if (!gladLoadGL(glfwGetProcAddress)) {
	 throw std::runtime_error("Glad failed to load");
	}
	std::cout << "Current Renderer: " << glGetString(GL_RENDERER) << std::endl;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return window;
}

void CubeRenderer::pollEvents() {
	glfwPollEvents();
}

//Should be the one to call glfwSwapBuffers()
void CubeRenderer::render(const CubeGame& game) {


	glfwSwapBuffers(window_);
}
