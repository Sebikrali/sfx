#define GLAD_GL_IMPLEMENTATION

#include "pch.h"

constexpr std::string APP_NAME = "SFX_GL";
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

typedef struct {
    float x, y, z;
} vec3;


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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}


int main() {
    GLFWwindow *window = init_glfw();
    if (!window) {
        Error_and_Exit("Couldn't initialize GLFW");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGL(glfwGetProcAddress)) {
        Error_and_Exit("Failed to initialize OpenGL context");
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Creating Objects
    std::array<vec3, 8> vertices({
        {-1.0f, -1.0f, 1.0f}, // bottom first, then clockwise
        {1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, 1.0f, 1.0f}, // top first
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, -1.0f},
    });

    std::array<uint8_t, 3*2*6> indices({
        0, 1, 5, // front
        1, 6, 5,
        2, 3, 6, // back
        3, 7, 6,
        3, 0, 7, // left
        0, 4, 7,
        1, 2, 5, // right
        2, 6, 5,
        4, 5, 7, // top
        5, 6, 7,
        0, 1, 3, // bottom
        1, 2, 3
    });

    // TODO: Create all OpenGL objects

    // TODO: Create all shaders

    // TODO: Render it

    while(!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // NOTE: Add dedicated cleanup function later
    glfwDestroyWindow(window);
    glfwTerminate();
}
