#include "window.hpp"

void app_sleep(float frame_rate) {
    static double frame_start = 0;
    double wait_time = 1.0 / frame_rate;

    double right_now = glfwGetTime();
    if (right_now - frame_start < wait_time) {
        double dur = (wait_time - (right_now - frame_start));
        long duration = long(std::round(dur * 1000.0));
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }
    frame_start = glfwGetTime();
}

void Window::init_window(const char * caption, uint16_t width, uint16_t height) {
    w_width = width;
    w_height = height;

    if (!glfwInit()) {
        send_error("window", "Failed to initialize GLFW library", -1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    send_info(2, "GLFW ", glfwGetVersionString());

    window = glfwCreateWindow(w_width, w_height, caption, NULL, NULL);
    if (!window) {
        glfwTerminate();
        send_error("window", "Failed to create GLFW window", -1);
    }
    glfwMakeContextCurrent(window);

    send_info(2, "Renderer ", glGetString(GL_RENDERER));
    send_info(2, "OpenGL version supported ", glGetString(GL_VERSION));

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        send_error("window", "Failed to initialize GLEW", -1);
    }

    glViewport(0, 0, w_width, w_height);
}

void Window::loop(float frame_rate) {
    if (init_callback) {
        init_callback();
    }
    if (keyboard_callback) {
        glfwSetKeyCallback(window, keyboard_callback);
    }
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (render_callback) {
            render_callback();
        }
        app_sleep(frame_rate);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

void Window::render(void (*render_func)(void)) {
    render_callback = render_func;
}

void Window::init_gl(void (*init_func)(void)) {
    init_callback = init_func;
}

void Window::keyboard(void (*keyboard_func)(GLFWwindow *, int, int, int, int)) {
    keyboard_callback = keyboard_func;
}