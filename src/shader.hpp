#pragma once
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();
        void create();
        void addShader(const char* fileName, GLenum shaderType);
        void link();

        void uniform(const char* varName, int value);
        void uniform(const char* varName, float value);
        void uniform(const char* varName, glm::vec2 value);
        void uniform(const char* varName, glm::vec4 value);

        void run();
        void stop();
    private:
        GLuint compileShader(const char* fileName, GLenum shaderType);
        GLuint program;
};
