#include "window.hpp"

GLfloat vert[] = {
    -0.5f, -0.5f, 0.0f,
    +0.5f, -0.5f, 0.0f,
    +0.0f, +0.5f, 0.0f
};
const GLchar * vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main() { gl_Position = vec4(position.x, position.y, position.z, 1.0); }";
const GLchar * fragmentShaderSource =
"#version 330 core\n"
"out vec4 color;\n"
"void main() { color = vec4(1.0f, 0.5f, 0.2f, 1.0f); }";

GLuint VBO;
GLuint VAO;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

void check_status(GLuint param, GLuint type) {
    const GLuint size = 1024;
    GLint success;
    GLchar log[size];

    if (type == GL_COMPILE_STATUS) {
        glGetShaderiv(param, type, &success);
        if (!success) {
            glGetShaderInfoLog(param, size, NULL, log);
            send_error("shader_compile", log, -1);
        }
    } else if (type == GL_LINK_STATUS) {
        glGetProgramiv(param, type, &success);
        if (!success) {
            glGetProgramInfoLog(param, size, NULL, log);
            send_error("program_link", log, -1);
        }
    }
}

GLuint compile_shader(const GLchar ** code, GLuint type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, code, NULL);
    glCompileShader(shader);

    check_status(shader, GL_COMPILE_STATUS);

    return shader;
}

void init(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    vertexShader = compile_shader(&vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShader = compile_shader(&fragmentShaderSource, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    check_status(shaderProgram, GL_LINK_STATUS);

    glUseProgram(shaderProgram);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glUseProgram(shaderProgram);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void deinit() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

int main() {
    Window window;

    window.init_window("test", 500, 500);
    window.init_gl(init);
    window.render(render);
    window.loop(30.0f);

    deinit();

    return 0;
}