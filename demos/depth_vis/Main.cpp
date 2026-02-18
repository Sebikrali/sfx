#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "sfx/Window.hpp"
#include "sfx/Timer.hpp"
#include "sfx/Import.hpp"
#include "sfx/Object.hpp"

int main() {
    Window window;
    auto rCtx = window.renderContext;
    rCtx->camera.setPos({ 0.0f, 0.0f, 3.0f });
    window.setupDebugHud("");

    auto shader = std::make_shared<Shader>("s.vert", "s.frag");

    // TODO: Make this more configurable via a keybind and maybe "poll" Near and Far from the window also during runtime
    shader->use();
    shader->setUniform("uNear", 0.1f);
    shader->setUniform("uFar", 100.0f);
    shader->setUniform("uStartDist", 0.0f);
    shader->setUniform("uEndDist", 5.0f);

    Object cube;
    cube.mesh = Mesh::Default();
    cube.shader = shader;


    auto imported_cat_mesh = Import::importAllMeshes("Cat.glb");
    std::cout << "Imported " << imported_cat_mesh.size() << " meshes\n";
    Object cat = imported_cat_mesh[0];
    cat.shader = shader;
    cat.mesh->m_modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Timer frame(true);
    while (!window.shouldClose()) {
        if (window.poll()) {
            continue;
        }
        float dt = frame.stopAndRestart();
        window.update(dt);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.draw({
            rCtx->camera.getViewProjMatrix(),
            rCtx->camera.getPos(),
            rCtx->lightMode,
            rCtx->drawNormalsUVs,
            {}
        });

        cat.draw({
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
