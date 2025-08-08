#include "Window.hpp"
#include "Debug.hpp"

Window::Window() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        error_and_exit("Couldn't initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL(glfwGetProcAddress)) {
        error_and_exit("Failed to initialize OpenGL context");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallbackGL, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE); // Disable notifications
    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    renderContext = std::make_shared<RenderContext>(Camera({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, 60.0f, (float) DEFAULT_WINDOW_WIDTH / (float) DEFAULT_WINDOW_HEIGHT, 0.1f, 100.0f));
    context = std::make_shared<WindowContext>();
}
 
Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::handleMovement(float dt) {
    glm::vec3 direction(0.0f);
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) direction += glm::vec3(0.0f, 0.0f, -1.0f);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) direction += glm::vec3(0.0f, 0.0f, 1.0f);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) direction += glm::vec3(-1.0f, 0.0f, 0.0f);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) direction += glm::vec3(1.0f, 0.0f, 0.0f);

    if (glm::length(direction) != 0) {
        renderContext->camera.move(direction, dt);
    }
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}


Window* Window::getWindow(GLFWwindow* window) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!self) {
        std::cerr << "Window pointer is invalid" << std::endl;
        assert(false);
    }
    return self;
}

void Window::error_callback(int error, const char* description) {
    std::cout << "[GLFW] Error: " << description << std::endl;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto ourWindow = Window::getWindow(window);
    WindowContext& windowContext = *ourWindow->context;
    RenderContext& renderContext = *ourWindow->renderContext;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (action != GLFW_PRESS) return; // NOTE: Maybe change to GLFW_RELEASE
    switch (key) {
        case GLFW_KEY_F3:
            renderContext.drawWireframe = !renderContext.drawWireframe;
            if (renderContext.drawWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case GLFW_KEY_F4:
            switch (renderContext.cullMode) {
                case BACK:
                    renderContext.cullMode = FRONT;
                    glCullFace(GL_FRONT); // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
                    break;
                case FRONT:
                    renderContext.cullMode = OFF;
                    glDisable(GL_CULL_FACE);
                    break;
                case OFF:
                    renderContext.cullMode = BACK;
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    break;
            }
            break;
        case GLFW_KEY_F:
            renderContext.cameraMode = (renderContext.cameraMode == FPS) ? FLY : FPS;
            renderContext.camera.setMode(renderContext.cameraMode);
            break;
        case GLFW_KEY_O:
            if (renderContext.cameraMode == ORBIT) return; // NOTE: Maybe this isn't needed / theres a better way
            renderContext.cameraMode = ORBIT;
            renderContext.camera.setMode(renderContext.cameraMode);
            break;
        case GLFW_KEY_U: 
            renderContext.drawNormalsUVs.y = abs(renderContext.drawNormalsUVs.y - 1.0f);
            break;
        case GLFW_KEY_N: 
            renderContext.drawNormalsUVs.x = abs(renderContext.drawNormalsUVs.x - 1.0f);
            break;
        case GLFW_KEY_1:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                renderContext.lightMode.x = abs(renderContext.lightMode.x - 1.0f);
            }
            break;
        case GLFW_KEY_2:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                renderContext.lightMode.y = abs(renderContext.lightMode.y - 1.0f);
            }
            break;
        case GLFW_KEY_3:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                renderContext.lightMode.z = abs(renderContext.lightMode.z - 1.0f);
            }
            break;
        case GLFW_KEY_TAB:
            windowContext.mouseCaptured = !windowContext.mouseCaptured;
            if (windowContext.mouseCaptured) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            break;
        case GLFW_KEY_SPACE:
            windowContext.fullscreen = !windowContext.fullscreen;
            if (windowContext.fullscreen) {
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, GLFW_DONT_CARE);
            } else {
                glfwSetWindowMonitor(window, nullptr, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, GLFW_DONT_CARE);
            }
            break;
        default:
            break;
    }
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    auto ourWindow = Window::getWindow(window);
    WindowContext& c = *ourWindow->context;
    RenderContext& rC = *ourWindow->renderContext;

    if (c.firstMouse) {
        c.xPos = xpos;
        c.yPos = ypos;
        c.firstMouse = false;
        return;
    }
    rC.camera.setView(glm::vec2(xpos - c.xPos, ypos - c.yPos));
    c.xPos = xpos;
    c.yPos = ypos;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto ourWindow = Window::getWindow(window);
    ourWindow->renderContext->camera.setZoom(yoffset);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto ourWindow = Window::getWindow(window);
    glViewport(0, 0, width, height);
    ourWindow->renderContext->camera.setAspectRatio((float) width / (float) height);
}

