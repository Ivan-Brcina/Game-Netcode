#include "cubeRenderer.hpp"
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

CubeRenderer::CubeRenderer() {
	window_ = openGLinit();
  shader_ = std::make_unique<Shader>(
      "Rendering/shader.vert",
      "Rendering/shader.frag");
	meshGenerator_.createCube(cubeVAO_, cubeVBO_);
	meshGenerator_.createPlane(planeVAO_, planeVBO_);
	shader_->use();
	view = glm::lookAt(glm::vec3(0.0f, 10.0f, 15.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	int vw = 0, vh = 0;
  glfwGetFramebufferSize(window_, &vw, &vh);
	projection = glm::perspective(glm::radians(60.0f), (float)vw / (float)vh, 0.1f, 200.0f);

	shader_->setUniformMat4("view", view);
	shader_->setUniformMat4("projection", projection);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

CubeRenderer::~CubeRenderer() {
	glDeleteVertexArrays(1, &planeVAO_);
	glDeleteBuffers(1, &planeVBO_);
	glDeleteVertexArrays(1, &cubeVAO_);
	glDeleteBuffers(1, &cubeVBO_);
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

GLFWwindow* CubeRenderer::getWindow() const {
	return window_;
}

void CubeRenderer::drawFloor() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f));
	shader_->setUniformVec3("color", glm::vec3(0.537f, 0.317f, 0.161f));
	shader_->setUniformMat4("model", model);

	glBindVertexArray(planeVAO_);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void CubeRenderer::drawCubes(const CubeGame& game) {
	glm::mat4 model = glm::mat4(1.0f);
	std::vector<glm::vec2> cubePositions = game.getPositions();
	std::vector<glm::vec3> cubeColors = game.getColors();

	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(cubePositions[i].x, 0.25f, cubePositions[i].y));
		shader_->setUniformVec3("color", cubeColors[i]);
		shader_->setUniformMat4("model", model);
		glBindVertexArray(cubeVAO_);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

}

//Should be the one to call glfwSwapBuffers()
void CubeRenderer::render(const CubeGame& game) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawFloor();
	drawCubes(game);
	glfwSwapBuffers(window_);

}
