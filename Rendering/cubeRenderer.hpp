#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../Cubes/cubeGame.hpp"

class CubeRenderer {
	private:
		GLFWwindow* window_;

		GLFWwindow* openGLinit();
	public:
		CubeRenderer();
		void pollEvents();
		GLFWwindow* getWindow();
		void render(const CubeGame& game);

};
