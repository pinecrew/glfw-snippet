#include <vector>
#include "window.hpp"
#include "shader.hpp"
#include "vertex.hpp"

std::vector<GLfloat> vert = {
    -0.5f, -0.5f, 0.0f,
    +0.5f, -0.5f, 0.0f,
    +0.5f, +0.5f, 0.0f,
    // -0.5f, +0.5f, 0.0f
};
ShaderProgram shader;
Vertex data;

/*
=======
// GLuint VBO;
// GLuint VAO;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

Vertex data;

>>>>>>> Stashed changes
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

*/
void init(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    shader.create();
    shader.addShader("vertex.glsl", GL_VERTEX_SHADER);
    shader.addShader("fragment.glsl", GL_FRAGMENT_SHADER);
    shader.link();
    shader.run();

    data.load_data(vert, 3, 3);
}

void deinit() {
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    shader.run();
    data.render(GL_TRIANGLE_STRIP);
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
