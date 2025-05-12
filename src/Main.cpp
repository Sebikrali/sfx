#define GLAD_GL_IMPLEMENTATION
#include "pch.h"

#include "Shader.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"

constexpr char APP_NAME[] = "SFX_GL";
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 768;

static bool g_wireframe = false;

Camera g_camera({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, 60.0f, (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);

// NOTE: Maybe i don't need this (reduce abstractions!)
void error_and_exit(const std::string& message) {
    std::cout << "[Main] Error: " << message << std::endl;
    exit(EXIT_FAILURE);
}


void error_callback(int error, const char* description) {
    std::cout << "[GLFW] Error: " << description << std::endl;
}

// TODO: Move all debug functions
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
static void APIENTRY DebugCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid *userParam) {
    std::string error = FormatDebugOutput(source, type, id, severity, message);
    std::cout << error << std::endl;
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

    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        g_wireframe = !g_wireframe;
        if (g_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

void handleMovement(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_camera.move(Direction::Forward, dt);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_camera.move(Direction::Backward, dt);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_camera.move(Direction::Left, dt);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_camera.move(Direction::Right, dt);
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

    glDebugMessageCallback(DebugCallbackGL, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

        // Creating Objects
        Geometry cube(
            GeometryData::Cube(1.0f),
            glm::mat4(1.0f),
            shader
        );

        // TODO: Implement the Camera

        float t = (float) glfwGetTime();
        float dt = 0.0f;
        while(!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            dt = t;
            t = (float) glfwGetTime();
            dt = t - dt;

            handleMovement(window, dt);
            glfwPollEvents();

            shader->use();
            shader->setUniform("viewProj", g_camera.getViewProjMatrix());

            cube.draw();
            glfwSwapBuffers(window);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}


static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
    std::string sourceString;
    switch (source) {
        case GL_DEBUG_SOURCE_API: 
            sourceString = "API";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceString = "Application";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceString = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceString = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceString = "Third Party";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceString = "Other";
            break;
        default:
            sourceString = "Unknown";
            break;
    }

    std::string typeString;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typeString = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeString = "Deprecated Behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeString = "Undefined Behavior";
            break;
            typeString = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeString = "Performance";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeString = "Other";
            break;
        default:
            typeString = "Unknown";
            break;
    }

    std::string severityString;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            severityString = "High";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severityString = "Medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severityString = "Low";
            break;
        default:
            severityString = "Unknown";
            break;
    }

    std::stringstream stringStream;
    stringStream << "OpenGL Error: " << msg;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";
    return stringStream.str();
}

