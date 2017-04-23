#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include "error.hpp"
#include <vector>

class Vertex {
public:
    void load_data(const std::vector<GLfloat> vertex, const GLint size, const GLsizei strides);
    void render(const GLuint type);
private:
    GLuint _size_count;
    GLuint _stride_count;
    GLuint VBO;
    GLuint VAO;
};