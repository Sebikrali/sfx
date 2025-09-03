#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "sfx/pch.h"

#include "sfx/Window.hpp"

#include "sfx/Timer.hpp"
#include "sfx/Uniforms.hpp"
#include "sfx/Object.hpp"
#include "sfx/Light.hpp"
#include "sfx/Shader.hpp"
#include "sfx/Material.hpp"
#include "sfx/Texture.hpp"
#include "sfx/Mesh.hpp"
#include "sfx/Import.hpp"

int main() {
    Window window;
    auto renderContext = window.renderContext;
    renderContext->camera.setPos({ 0.0f, 0.0f, 5.0f });
    window.setupDebugHud("assets/fonts/JetBrainsMono-Medium.ttf");

    {
        auto importedObjects = Import::importAllMeshes("assets/models/sphere.ply");

        std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        std::shared_ptr<Shader> universalShader = std::make_shared<Shader>("assets/shaders/universal.vert", "assets/shaders/universal.frag");
        std::shared_ptr<Shader> textureShader = std::make_shared<Shader>("assets/shaders/texture.vert", "assets/shaders/texture.frag");
        std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");

        std::shared_ptr<Shader> textShader = std::make_shared<Shader>("assets/shaders/text.vert", "assets/shaders/text.frag");

        std::vector<std::shared_ptr<Shader>> shaders { shader, universalShader, textureShader, lightingShader };
        
        Texture texture("assets/textures/container.jpg");
        Material material({0.2f, 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f});
        PointLight light{ {0.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.09, 0.032} };

        Object object = Object::Default();

        Mesh lightCube(
            MeshData::Cube(0.5f),
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 2.0f))
        );

        Mesh plane(
            MeshData::Plane(5.0f),
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f))
        );

        Mesh cube(
            MeshData::Cube(1.0f),
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f))
        );

        Mesh rect(
            MeshData::Rectangle(),
            glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f))
        );

        Mesh cylinder(
            MeshData::Cylinder(),
            glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 0.0f))
        );

        Mesh sphere(
            MeshData::Sphere(),
            glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 3.0f))
        );

        Timer frame(true);
        while(!window.shouldClose()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            float dt = frame.stopAndRestart();

            window.handleMovement(dt);
            glfwPollEvents();

            RenderUniforms uniforms = {
                renderContext->camera.getViewProjMatrix(),
                renderContext->camera.getPos(),
                renderContext->lightMode,
                renderContext->drawNormalsUVs,
                renderContext->hideMaterialTexture,
                light
            };

            for (const auto& s : shaders) {
                s->use();
                s->setUniform("viewProj", uniforms.viewProj);
                s->setUniform("viewPos", uniforms.viewPos);
                s->setUniform("lightMode", uniforms.lightMode);
                s->setUniform("drawNormalsUVs", uniforms.drawNormalsUVs);
                s->setUniform("hideMaterialTexture", uniforms.hideMaterialTexture);
                s->setUniform("pointLight", uniforms.pointLight);
            }

            material.use(lightingShader);
            material.use(universalShader);

            plane.draw(lightingShader);

            lightCube.draw(shader);

            object.draw(uniforms);

            texture.use();
            cube.draw(lightingShader);
            rect.draw(textureShader);
            cylinder.draw(lightingShader);
            sphere.draw(universalShader);

            for (const auto& obj : importedObjects) {
                obj.draw(uniforms);
            }

            window.drawDebugHud();

            glfwSwapBuffers(window.m_window);
        }
    }

    return EXIT_SUCCESS;
}

