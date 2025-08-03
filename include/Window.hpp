#pragma once

#include "pch.h"
#include "Context.hpp"

constexpr char APP_NAME[] = "SFX_GL";
constexpr int DEFAULT_WINDOW_WIDTH = 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 768;

struct Window {
    GLFWwindow* m_window;
    std::shared_ptr<WindowContext> context;
    std::shared_ptr<RenderContext> renderContext;

    Window();
    ~Window();

    void handleMovement(float dt);

    bool shouldClose() const;

private:
    static Window* getWindow(GLFWwindow* window);
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
