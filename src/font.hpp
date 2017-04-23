#pragma once
#include <iostream>
#include <cstdint>
#include <map>
#include <GL/glew.h>
// OpenGL
#include <GL/gl.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader.hpp"
#include "error.hpp"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;    // ID handle of the glyph texture
    glm::ivec2 Size;     // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;      // Horizontal offset to advance to next glyph
};

class Font {
public:
    void load(const char * font_name, const uint16_t size);
    void render(std::string text, glm::vec3 pos, GLfloat scale);
private:
    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
};