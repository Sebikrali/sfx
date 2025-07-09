#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "pch.h"

#include "Debug.hpp"
#include "DebugUtils.hpp"

#include "Light.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"

constexpr char APP_NAME[] = "SFX_GL";
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 768;

enum CullMode {
    BACK,
    FRONT,
    OFF
};

static bool g_wireframe = false;
static CullMode g_cull = BACK;
static bool g_firstMouse = true;
static glm::vec3 g_lightMode = { 1.0f, 1.0f, 1.0f };

Camera g_camera({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, 60.0f, (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
static CameraMode g_cameraMode = FPS;

static double xPos = 0.0;
static double yPos = 0.0;

// Probably write this into another file
void error_callback(int error, const char* description) {
    std::cout << "[GLFW] Error: " << description << std::endl;
}

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
        return;
    }

    if (action != GLFW_PRESS) return; // NOTE: Maybe change to GLFW_RELEASE
    switch (key) {
        case GLFW_KEY_F3:
            g_wireframe = !g_wireframe;
            if (g_wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case GLFW_KEY_F4:
            switch (g_cull) {
                case BACK:
                    g_cull = FRONT;
                    glCullFace(GL_FRONT); // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
                    break;
                case FRONT:
                    g_cull = OFF;
                    glDisable(GL_CULL_FACE);
                    break;
                case OFF:
                    g_cull = BACK;
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    break;
            }
            break;
        case GLFW_KEY_F:
            g_cameraMode = (g_cameraMode == FPS) ? FLY : FPS;
            g_camera.setMode(g_cameraMode);
            break;
        case GLFW_KEY_O:
            if (g_cameraMode == ORBIT) return; // NOTE: Maybe this isn't needed / theres a better way
            g_cameraMode = ORBIT;
            g_camera.setMode(g_cameraMode);
            break;
        case GLFW_KEY_1:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                g_lightMode.x = abs(g_lightMode.x - 1.0f);
            }
            break;
        case GLFW_KEY_2:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                g_lightMode.y = abs(g_lightMode.y - 1.0f);
            }
            break;
        case GLFW_KEY_3:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                g_lightMode.z = abs(g_lightMode.z - 1.0f);
            }
            break;
        default:
            break;
    }
}

void handleMovement(GLFWwindow* window, float dt) {
    glm::vec3 direction(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction += glm::vec3(0.0f, 0.0f, -1.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction += glm::vec3(0.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction += glm::vec3(-1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction += glm::vec3(1.0f, 0.0f, 0.0f);

    if (glm::length(direction) != 0) {
        g_camera.move(direction, dt);
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (g_firstMouse) {
        xPos = xpos;
        yPos = ypos;
        g_firstMouse = false;
        return;
    }
    g_camera.setView(glm::vec2(xpos - xPos, ypos - yPos));
    xPos = xpos;
    yPos = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_camera.setZoom(yoffset);
}

int main() {
    GLFWwindow *window = init_glfw();
    if (!window) {
        error_and_exit("Couldn't initialize GLFW");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL(glfwGetProcAddress)) {
        error_and_exit("Failed to initialize OpenGL context");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallbackGL, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE); // Disable notifications
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        std::shared_ptr<Shader> textureShader = std::make_shared<Shader>("assets/shaders/texture.vert", "assets/shaders/texture.frag");
        std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");
        
        Texture texture("assets/textures/container.jpg");
        Material material({0.2f, 0.5f, 0.5f}, 0.5f, {1.0f, 0.0f, 0.0f});
        PointLight light{ {0.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.09, 0.032} };

        Geometry lightCube(
            GeometryData::Cube(0.5f),
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 2.0f)),
            shader
        );

        // Creating Objects
        Geometry plane(
            GeometryData::Plane(5.0f),
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f)),
            lightingShader
        );

        Geometry cube(
            GeometryData::Cube(1.0f),
            glm::mat4(1.0f),
            textureShader
        );

        Geometry rect(
            GeometryData::Rectangle(),
            glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f)),
            textureShader
        );

        Geometry cylinder(
            GeometryData::Cylinder(),
            glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 0.0f)),
            textureShader
        );

        Geometry sphere(
            GeometryData::Sphere(),
            glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 3.0f)),
            textureShader // shader
        );


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

            textureShader->use();
            textureShader->setUniform("viewProj", g_camera.getViewProjMatrix());

            lightingShader->use();
            lightingShader->setUniform("viewProj", g_camera.getViewProjMatrix());
            lightingShader->setUniform("viewPos", g_camera.getPos());
            lightingShader->setUniform("g_lightMode", g_lightMode);
            lightingShader->setUniform("pointLight", light);
            material.use(lightingShader);

            plane.draw();

            lightCube.draw();

            texture.draw();
            cube.draw();
            rect.draw();
            cylinder.draw();
            sphere.draw();
            glfwSwapBuffers(window);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

