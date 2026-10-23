#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../Cubes/cubeGame.hpp"
#include "mesh.hpp"

class CubeRenderer {
	private:
		GLFWwindow* window_;
		Mesh meshGenerator_;
		GLuint planeVAO_, planeVBO_, cubeVAO_, cubeVBO_;

		GLFWwindow* openGLinit();
	public:
		CubeRenderer();
		void pollEvents();
		GLFWwindow* getWindow();
		void render(const CubeGame& game);
};
