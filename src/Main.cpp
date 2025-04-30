#define GLAD_GL_IMPLEMENTATION
#include "pch.h"

#include "Shader.hpp"
#include "Geometry.hpp"

constexpr char APP_NAME[] = "SFX_GL";
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;


// NOTE: Maybe i don't need this (reduce abstractions!)
void error_and_exit(const std::string& message) {
    std::cout << "[Main] Error: " << message << std::endl;
    exit(EXIT_FAILURE);
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

    return glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}


int main() {
    GLFWwindow *window = init_glfw();
    if (!window) {
        error_and_exit("Couldn't initialize GLFW");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGL(glfwGetProcAddress)) {
        error_and_exit("Failed to initialize OpenGL context");
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    {

        std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

        // Creating Objects
        Geometry cube(
            GeometryData::Cube(1.0f),
            glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -1.0f}),
            shader
        );

        // TODO: Next up: Create the Camera


        // TODO: Add timestamps for framerate independence
        while(!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            glfwPollEvents();

            cube.draw();
            glfwSwapBuffers(window);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
