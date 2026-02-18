#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "sfx/Window.hpp"
#include "sfx/Timer.hpp"

int main() {
    Window window;

    glClearColor(0.019f, 0.086f, 0.309f, 1.0f);
    Timer frame(true);
    while (!window.shouldClose()) {
        if (window.poll()) {
            continue;
        }
        float dt = frame.stopAndRestart();
        window.update(dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window.m_window);
    }

}

