#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "pch.h"

#include "Window.hpp"

#include "Timer.hpp"
#include "Uniforms.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

int main() {
    Window window;
    auto renderContext = window.renderContext;

    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        std::shared_ptr<Shader> textureShader = std::make_shared<Shader>("assets/shaders/texture.vert", "assets/shaders/texture.frag");
        std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");

        std::vector<std::shared_ptr<Shader>> shaders { shader, textureShader, lightingShader };
        
        Texture texture("assets/textures/container.jpg");
        Material material({0.2f, 0.5f, 0.5f}, 0.5f, {1.0f, 0.0f, 0.0f});
        PointLight light{ {0.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.09, 0.032} };

        Object object = Object::Default();

        Mesh lightCube(
            MeshData::Cube(0.5f),
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 2.0f))
        );

        // Creating Objects
        std::shared_ptr<Mesh> defaultMesh = Mesh::Default();

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
                light
            };

            for (const auto& s : shaders) {
                s->use();
                s->setUniform("viewProj", uniforms.viewProj);
                s->setUniform("viewPos", uniforms.viewPos);
                s->setUniform("lightMode", uniforms.lightMode);
                s->setUniform("drawNormalsUVs", uniforms.drawNormalsUVs);
                s->setUniform("pointLight", uniforms.pointLight);
            }

            material.use(lightingShader);

            plane.draw(lightingShader);

            lightCube.draw(shader);

            object.draw(uniforms);

            texture.use();
            cube.draw(lightingShader);
            rect.draw(textureShader);
            cylinder.draw(lightingShader);
            sphere.draw(textureShader);

            // TODO: This will need to go elsewhere i think
            glfwSwapBuffers(window.m_window);
        }
    }

    return EXIT_SUCCESS;
}

