#include <vector>
#include "window.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "loader.hpp"

ShaderProgram shader;
Vertex data;
Vertex point;

flat_data_t * draw_data;

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
    shader.run();

    draw_data = load_data("dump.bin");

    auto r = find_area_radius(draw_data);
    auto grid = generate_grid(20, 10, r);
    data.load_data(grid, 2);
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

    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);

    shader.uniform("ourColor", glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));

    // don't do that
    glPointSize(5.0f);

    auto frame_size = 2 * draw_data->particle_count;
    uint32_t current_frame_start = current_frame * frame_size;

    point.load_data(draw_data->data + current_frame_start, draw_data->data + current_frame_start + frame_size, 2);
    point.render(GL_POINTS);

    if (current_frame > draw_data->frame_count) {
        current_frame = 0;
    } else {
        current_frame++;
    }
}

int main() {
    Window window;

    window.init_window("Flat Render Demo", 500, 500);
    window.init_gl(init);
    window.render(render);
    window.loop(30.0f);

    deinit();

    return 0;
}
