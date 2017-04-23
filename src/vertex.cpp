#include "vertex.hpp"

void Vertex::load_data(const std::vector<GLfloat> vertex, const GLint size, const GLsizei strides) {
    _size_count = size;
    _stride_count = strides;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, strides * sizeof(GLfloat), (GLvoid *)0);

    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, strides * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Vertex::render(const GLuint type) {
    glBindVertexArray(VAO);
    glDrawArrays(type, 0, _stride_count);
    glBindVertexArray(0);
}