#pragma once
#include <GLFW/glfw3.h>
#include "cubeGame.hpp"

class CubeRenderer {
	private:
		GLuint window_;

		void openGLinit();
	public:
		void pollEvents();
		GLFWwindow* getWindow();
		void render(const CubeGame& game);

};
