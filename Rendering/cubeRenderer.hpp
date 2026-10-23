#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../Cubes/cubeGame.hpp"
#include "mesh.hpp"
#include "shaders.hpp"

class CubeRenderer {
	private:
		GLFWwindow* window_;
		Mesh meshGenerator_;
		GLuint planeVAO_, planeVBO_, cubeVAO_, cubeVBO_;
		std::unique_ptr<Shader> shader_;

		GLFWwindow* openGLinit();
	public:
		CubeRenderer();
		~CubeRenderer();
		void pollEvents();
		GLFWwindow* getWindow();
		void render(const CubeGame& game);
};
