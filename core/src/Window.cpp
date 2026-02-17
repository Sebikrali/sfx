#include "sfx/Window.hpp"
#include "sfx/Debug.hpp"
#include "sfx/pch.h"

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
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL(glfwGetProcAddress)) {
        error_and_exit("Failed to initialize OpenGL context");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // NOTE: Strip out for release builds
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

    renderContext = std::make_shared<RenderContext>(Camera({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, 60.0f, (float) DEFAULT_WINDOW_WIDTH / (float) DEFAULT_WINDOW_HEIGHT, 0.1f, 100.0f));
    context = std::make_shared<WindowContext>();
}
 
Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::update(float dt) {
    handleMovement(dt);

    // TODO: Extend to show average, 1% low, etc.
    numFrames++;
    timeElapsed += dt;
    if (timeElapsed >= 1.0f) {
        frameTime = 1000.0 / numFrames;
        timeElapsed -= 1.0f;
        numFrames = 0;
    }
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

void Window::setupDebugHud(const std::string& fontPath) {
    fontManager.setFont(fontPath);

    debugHud = fontManager.createTextCollectionPtr();
    debugHud->shader = Shader::TextShader();

    debugHud->dynamicTexts.emplace("modes", DynamicText{"debug modes: ", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 768.0f - fontManager.fontSize, 0.0f))});
    debugHud->dynamicTexts.emplace("pos", DynamicText{"pos: (0.0, 0.0, 0.0)", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 768.0f - fontManager.fontSize * 2, 0.0f))});
    debugHud->dynamicTexts.emplace("view", DynamicText{"view dir: (0.0, 0.0, 0.0)", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 768.0f - fontManager.fontSize * 3, 0.0f))});
    debugHud->dynamicTexts.emplace("fps", DynamicText{"frameTime: 0.0ms", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 768.0f - fontManager.fontSize * 4, 0.0f))});
}

void Window::drawDebugHud() {
    if (!debugHud || !renderContext->drawDebugHud) {
        return;
    }

    glm::mat4 projection = glm::ortho(0.0f, (float) context->width, 0.0f, (float) context->height);
    debugHud->projection = projection;
    auto pos = renderContext->camera.getPos();
    debugHud->dynamicTexts["pos"].value = std::format("pos: ({:.2f},{:.2f},{:.2f})", pos.x, pos.y, pos.z);
    auto view = renderContext->camera.getView();
    debugHud->dynamicTexts["view"].value = std::format("view dir: ({:.2f},{:.2f},{:.2f})", view.x, view.y, view.z);
    debugHud->dynamicTexts["fps"].value = std::format("frameTime: {:.4f}ms", frameTime);

    // cullMode F4, cameraMode F/O, uvs U, normals N, material M, texture T, lightmode L+123
    std::string debugText = "debug modes:";
    switch (renderContext->cullMode) {
        case BACK:
            debugText += " culling_back";
            break;
        case FRONT:
            debugText += " culling_front";
            break;
        case OFF:
            break;
    }
    if (renderContext->drawNormalsUVs.x == 1.0f) debugText += " normals";
    if (renderContext->drawNormalsUVs.y == 1.0f) debugText += " uvs";
    if (renderContext->hideMaterialTexture.x == 1.0f) debugText += " hide_mat";
    if (renderContext->hideMaterialTexture.y == 1.0f) debugText += " hide_tex";
    switch (renderContext->cameraMode) {
        case FLY:
            debugText += " camera_fly";
            break;
        case FPS:
            debugText += " camera_fps";
            break;
        case ORBIT:
            debugText += " camera_orbit";
            break;
    }
    if (renderContext->lightMode.w == 0.0f) debugText += " lighting_off";
    if (renderContext->lightMode.x == 1.0f) debugText += " ambient";
    if (renderContext->lightMode.y == 1.0f) debugText += " diffuse";
    if (renderContext->lightMode.z == 1.0f) debugText += " specular";
    debugHud->dynamicTexts["modes"].value = debugText;

    debugHud->draw();
}

void Window::setClearColor(Color color) {
    const glm::vec4& c = color.toFloat();
    glClearColor(c.r, c.g, c.b, c.a);
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

void printUsage() {
    std::cout << "\nKeybindings:\n";
    std::cout << "\tESC - quit\n";
    std::cout << "\tTAB - toggle mouse capture off/on\n";
    std::cout << "\tSpace - toggle fullscreen on/off\n";
    std::cout << "\tF - toggle between fly and fps camera mode\n";
    std::cout << "\tO - activate orbit camera mode\n";
    std::cout << "\tU - toggle drawing texture coords(UVs)\n";
    std::cout << "\tN - toggle drawing normals\n";
    std::cout << "\tM - toggle material drawing\n";
    std::cout << "\tT - toggle texture drawing\n";
    std::cout << "\tL+(0|1|2|3) - toggle light types off/on: 0 = all off, 1 = ambient, 2 = diffuse 3 = specular\n";
    std::cout << "\tF1 - print this help message to stdout\n";
    std::cout << "\tF3 - toggle wireframe on/off\n";
    std::cout << "\tF4 - toggle cullmode front/off/back\n";
    std::cout << "\tF10 - toggle debug hud on/off\n";
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto ourWindow = Window::getWindow(window);
    WindowContext& ctx = *ourWindow->context;
    RenderContext& renderCtx = *ourWindow->renderContext;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (action != GLFW_PRESS) return; // NOTE: Maybe change to GLFW_RELEASE
    switch (key) {
        case GLFW_KEY_TAB:
            ctx.mouseCaptured = !ctx.mouseCaptured;
            if (ctx.mouseCaptured) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            break;
        case GLFW_KEY_SPACE:
            ctx.fullscreen = !ctx.fullscreen;
            if (ctx.fullscreen) {
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, GLFW_DONT_CARE);
            } else {
                glfwSetWindowMonitor(window, nullptr, 0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, GLFW_DONT_CARE);
            }
            break;
        case GLFW_KEY_F:
            renderCtx.cameraMode = (renderCtx.cameraMode == FPS) ? FLY : FPS;
            renderCtx.camera.setMode(renderCtx.cameraMode);
            break;
        case GLFW_KEY_O:
            if (renderCtx.cameraMode == ORBIT) return; // NOTE: Maybe this isn't needed / theres a better way
            renderCtx.cameraMode = ORBIT;
            renderCtx.camera.setMode(renderCtx.cameraMode);
            break;
        case GLFW_KEY_U: 
            renderCtx.drawNormalsUVs.y = abs(renderCtx.drawNormalsUVs.y - 1.0f);
            break;
        case GLFW_KEY_N: 
            renderCtx.drawNormalsUVs.x = abs(renderCtx.drawNormalsUVs.x - 1.0f);
            break;
        case GLFW_KEY_M: 
            renderCtx.hideMaterialTexture.x = abs(renderCtx.hideMaterialTexture.x - 1.0f);
            break;
        case GLFW_KEY_T: 
            renderCtx.hideMaterialTexture.y = abs(renderCtx.hideMaterialTexture.y - 1.0f);
            break;
        case GLFW_KEY_0:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                renderCtx.lightMode.w = abs(renderCtx.lightMode.w - 1.0f);
            }
            break;
        case GLFW_KEY_1:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                renderCtx.lightMode.x = abs(renderCtx.lightMode.x - 1.0f);
            }
            break;
        case GLFW_KEY_2:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                renderCtx.lightMode.y = abs(renderCtx.lightMode.y - 1.0f);
            }
            break;
        case GLFW_KEY_3:
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                renderCtx.lightMode.z = abs(renderCtx.lightMode.z - 1.0f);
            }
            break;
        case GLFW_KEY_F1:
            printUsage();
            break;
        case GLFW_KEY_F3:
            renderCtx.drawWireframe = !renderCtx.drawWireframe;
            if (renderCtx.drawWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
        case GLFW_KEY_F4:
            switch (renderCtx.cullMode) {
                case BACK:
                    renderCtx.cullMode = FRONT;
                    glCullFace(GL_FRONT); // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
                    break;
                case FRONT:
                    renderCtx.cullMode = OFF;
                    glDisable(GL_CULL_FACE);
                    break;
                case OFF:
                    renderCtx.cullMode = BACK;
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    break;
            }
            break;
        case GLFW_KEY_F10:
            renderCtx.drawDebugHud = !renderCtx.drawDebugHud;
            break;
        default:
            break;
    }
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto ourWindow = Window::getWindow(window);
    WindowContext& ctx = *ourWindow->context;

    if (ctx.mouseCaptured) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ctx.dragging = true;
        ctx.firstMouse = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        ctx.dragging = false;
    }
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    auto ourWindow = Window::getWindow(window);
    WindowContext& ctx = *ourWindow->context;
    RenderContext& renderCtx = *ourWindow->renderContext;

    if (ctx.firstMouse) {
        ctx.xPos = xpos;
        ctx.yPos = ypos;
        ctx.firstMouse = false;
        return;
    }
    if (!ctx.mouseCaptured && !ctx.dragging) {
        return;
    }

    renderCtx.camera.updateView(glm::vec2(xpos - ctx.xPos, ypos - ctx.yPos));
    ctx.xPos = xpos;
    ctx.yPos = ypos;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto ourWindow = Window::getWindow(window);
    ourWindow->renderContext->camera.zoom(yoffset);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto ourWindow = Window::getWindow(window);
    glViewport(0, 0, width, height);
    ourWindow->renderContext->camera.setAspectRatio((float) width / (float) height);
    ourWindow->context->width = width;
    ourWindow->context->height = height;
}

