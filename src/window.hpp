#pragma once
#include <chrono>
#include <cmath>
#include <stdint.h>
#include <string>
#include <thread>
//
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// ...
#include "error.hpp"

class Window {
public:
    void init_window(const char * caption, uint16_t width, uint16_t height);
    void loop(float fps);
    void render(void (*rend_func)(Window * window));
    void init_gl(void (*init_func)(void));
    void keyboard(void (*keyboard_func)(GLFWwindow *, int, int, int, int));
    float get_fps();
private:
    void window_sleep();
    float fps;
    float real_fps;
    uint16_t w_width;
    uint16_t w_height;
    GLFWwindow * window = nullptr;
    void (*render_callback)(Window * window);
    void (*init_callback)(void);
    void (*keyboard_callback)(GLFWwindow *, int, int, int, int);
};