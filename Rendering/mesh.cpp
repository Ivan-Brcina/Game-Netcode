#include "mesh.hpp"
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh() {}

const std::vector<float> cubeVertices = {
	//Position     //TexCoords
	-0.5,-0.5,-0.5, 0.0,0.0,
	 0.5,-0.5,-0.5, 1.0,0.0,
	 0.5, 0.5,-0.5, 1.0,1.0,
	 0.5, 0.5,-0.5, 1.0,1.0,
	 0.5, 0.5,-0.5, 0.0,1.0,
	-0.5,-0.5,-0.5, 0.0,0.0,

	-0.5,-0.5, 0.5, 0.0,0.0,
	 0.5,-0.5, 0.5, 1.0,0.0,
	 0.5, 0.5, 0.5, 1.0,1.0,
	 0.5, 0.5, 0.5, 1.0,1.0,
	-0.5, 0.5, 0.5, 0.0,1.0,
	-0.5,-0.5, 0.5, 0.0,0.0,

	-0.5, 0.5, 0.5, 1.0,0.0,
	-0.5, 0.5,-0.5, 1.0,1.0,
	-0.5,-0.5,-0.5, 0.0,1.0,
	-0.5,-0.5,-0.5, 0.0,1.0,
	-0.5,-0.5, 0.5, 0.0,0.0,
	-0.5, 0.5, 0.5, 1.0,0.0,

	 0.5, 0.5, 0.5, 1.0,0.0,
	 0.5, 0.5,-0.5, 1.0,1.0,
	 0.5,-0.5,-0.5, 0.0,1.0,
	 0.5,-0.5,-0.5, 0.0,1.0,
	 0.5,-0.5, 0.5, 0.0,0.0,
	 0.5, 0.5, 0.5, 1.0,0.0,

	-0.5,-0.5,-0.5, 0.0,1.0,
	 0.5,-0.5,-0.5, 1.0,1.0,
	 0.5,-0.5, 0.5, 1.0,0.0,
	 0.5,-0.5, 0.5, 1.0,0.0,
	-0.5,-0.5, 0.5, 0.0,0.0,
	-0.5,-0.5,-0.5, 0.0,1.0,

	-0.5, 0.5,-0.5, 0.0,1.0,
	 0.5, 0.5,-0.5, 1.0,1.0,
	 0.5, 0.5, 0.5, 1.0,0.0,
	 0.5, 0.5, 0.5, 1.0,0.0,
	-0.5, 0.5, 0.5, 0.0,0.0,
	-0.5, 0.5,-0.5, 0.0,1.0
};

const std::vector<float> planeVertices {
 // positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};


void Mesh::createCube(GLuint& vao, GLuint& vbo) {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glBindVertexArray(0);
}

void Mesh::createPlane(GLuint&vao, GLuint& vbo) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(float), planeVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}






