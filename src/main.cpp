#define GLAD_GL_IMPLEMENTATION

#include "pch.h"

constexpr std::string APP_NAME = "SFX_GL";
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

// NOTE: Maybe i don't need this (reduce abstractions!)
void Error_and_Exit(const std::string& message) {
    std::cout << "[Main] Error: " << message << std::endl;
    exit(1);
}


void error_callback(int error, const char* description) {
    std::cout << "[GLFW] Error: " << description << std::endl;
}

// Probably write this into another file
GLFWwindow *init_glfw() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME.c_str(), nullptr, nullptr);
}


int main() {
    GLFWwindow *window = init_glfw();
    if (!window) {
        Error_and_Exit("Couldn't initialize GLFW");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        Error_and_Exit("Failed to initialize OpenGL context");
    }

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
