#include <vector>
#include "window.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "loader.hpp"
#include "font.hpp"

ShaderProgram shader;
ShaderProgram text;
Vertex data;
Vertex point;

Font font;

flat_data_t * draw_data;
bool pause_flag = false;

const uint16_t w_width = 500;
const uint16_t w_height = 500;

std::vector<GLfloat> generate_grid(GLuint vlines, GLuint hlines, float r) {
    const uint32_t ncoords = 4;
    const uint32_t elements_count = ncoords * (vlines + hlines);
    const float vgrid_step = 2.0 * r / ((float) hlines - 1.0);
    const float hgrid_step = 2.0 * r / ((float) vlines - 1.0);
    std::vector<GLfloat> grid_data;

    grid_data.resize(elements_count);

    for (uint32_t i = 0; i < vlines; ++i) {
        // vertical lines
        grid_data[i * ncoords + 0] = -r + hgrid_step * i;
        grid_data[i * ncoords + 1] = -r;
        grid_data[i * ncoords + 2] = -r + hgrid_step * i;
        grid_data[i * ncoords + 3] = r;
    }
    uint32_t first_part = vlines * ncoords;
    for (uint32_t i = 0; i < hlines; ++i) {
        // horizontal lines
        grid_data[first_part + i * ncoords + 0] = -r;
        grid_data[first_part + i * ncoords + 1] = -r + vgrid_step * i;
        grid_data[first_part + i * ncoords + 2] = r;
        grid_data[first_part + i * ncoords + 3] = -r + vgrid_step * i;
    }

    return grid_data;
}

float find_area_radius(flat_data_t * data) {
    float r2 = data->data[0] * data->data[0] + data->data[1] * data->data[1];
    for (uint32_t i = 2; i < 2 * data->frame_count * data->particle_count; i += 2) {
        float _r2 = data->data[i] * data->data[i] + data->data[i+1] * data->data[i+1];
        if (_r2 > r2) { r2 = _r2; }
    }
    return std::sqrt(r2);
}

void init(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    shader.create();
    shader.addShader("vertex.glsl", GL_VERTEX_SHADER);
    shader.addShader("fragment.glsl", GL_FRAGMENT_SHADER);
    shader.link();

    text.create();
    text.addShader("vertex_text.glsl", GL_VERTEX_SHADER);
    text.addShader("fragment_text.glsl", GL_FRAGMENT_SHADER);
    text.link();

    draw_data = load_data("dump.bin");

    auto r = find_area_radius(draw_data);
    auto grid = generate_grid(20, 10, r);
    data.load_data(grid, 2);

    font.load("FiraSans-Medium.ttf", 32);
}

void deinit() {
}

void render(void) {
    static uint32_t current_frame = 0;

    glClear(GL_COLOR_BUFFER_BIT);

    shader.run();
    shader.uniform("ourColor", glm::vec4(1.0f, 1.0f, 1.0f, 0.2f));

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    data.render(GL_LINES);

    shader.uniform("ourColor", glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));

    // don't do that
    glPointSize(5.0f);

    auto frame_size = 2 * draw_data->particle_count;
    uint32_t current_frame_start = current_frame * frame_size;

    point.load_data(draw_data->data + current_frame_start, draw_data->data + current_frame_start + frame_size, 2);
    point.render(GL_POINTS);

    text.run();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w_width), 0.0f, static_cast<GLfloat>(w_height));
    text.uniform("projection", projection);
    text.uniform("textColor", glm::vec3(1.0f, 1.0f, 1.0f));
    font.render("test", glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);

    if (!pause_flag) {
        if (current_frame > draw_data->frame_count) {
            current_frame = 0;
        } else {
            current_frame++;
        }
    }
}

void keyboard(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pause_flag = !pause_flag;
    }
}

int main() {
    Window window;

    window.init_window("Flat Render Demo", w_width, w_height);
    window.init_gl(init);
    window.render(render);
    window.keyboard(keyboard);
    window.loop(30.0f);

    deinit();

    return 0;
}
