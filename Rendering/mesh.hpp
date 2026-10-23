#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

class Mesh {

  public:
    Mesh();

    void createPlane(GLuint& vao, GLuint& vbo);
    void createCube(GLuint& vao, GLuint& vbo);

};
