#pragma once

#include "pch.h"
#include "Context.hpp"
#include "Text.hpp"
#include "Color.hpp"

constexpr char APP_NAME[] = "SFX_GL";
constexpr int DEFAULT_WINDOW_WIDTH = 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 768;

struct Window {
    GLFWwindow* m_window;
    std::shared_ptr<WindowContext> context;
    std::shared_ptr<RenderContext> renderContext;
    std::shared_ptr<TextCollection> debugHud;
    FontManager fontManager;

    Window();
    ~Window();

    /**
     * @brief Polls window and key updates, also sleeps for 30ms if window is out of focus.
     * @return true - if the window has slept and a new poll should be done, false - otherwise.
     */
    bool poll();
    void update(float dt);

    void setupDebugHud(const std::string& fontPath);
    void drawDebugHud();

    void setClearColor(Color color);

    bool shouldClose() const;

private:
    // time since last tick in seconds
    float timeElapsed = .0f;
    uint32_t numFrames = 0u;
    float frameTime = .0f;

    void handleMovement(float dt);

    static Window* getWindow(GLFWwindow* window);
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void focus_callback(GLFWwindow* window, int focused);
};
