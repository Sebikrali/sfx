#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "sfx/Window.hpp"
#include "sfx/Timer.hpp"
#include "sfx/Object.hpp"

int main() {
    Window window;
    auto rCtx = window.renderContext;
    rCtx->camera.setPos({ 0.0f, 0.0f, 3.0f });
    window.setupDebugHud("");

    Object cube;
    cube.mesh = Mesh::Default();
    cube.mesh->m_modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f));
    cube.shader = std::make_shared<Shader>("shader.vert", "shader.frag");

    Timer frame(true);
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float dt = frame.stopAndRestart();

        window.handleMovement(dt);
        glfwPollEvents();

        cube.draw({
            rCtx->camera.getViewProjMatrix(),
            rCtx->camera.getPos(),
            rCtx->lightMode,
            rCtx->drawNormalsUVs,
            {}
        });

        window.drawDebugHud();

        glfwSwapBuffers(window.m_window);
    }
}
