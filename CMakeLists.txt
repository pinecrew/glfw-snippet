cmake_minimum_required(VERSION 2.6)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Freetype REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)
add_executable(ex-window src/window.cpp src/error.cpp src/main.cpp)
target_compile_features(ex-window PRIVATE cxx_aggregate_default_initializers)
target_link_libraries(ex-window 
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)