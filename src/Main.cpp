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

#include "Text.hpp"

int main() {
    Window window;
    auto renderContext = window.renderContext;

    {
        FontManager fontManager;
        auto fontSize = fontManager.fontSize;
        TextCollection textCollection = fontManager.createTextCollection();
        textCollection.dynamicTexts.emplace("modes", DynamicText{"debug modes: ", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 768.0f - fontSize, 0.0f))});
        textCollection.dynamicTexts.emplace("pos", DynamicText{"pos: (0.0, 0.0, 0.0)", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 768.0f - fontSize * 2, 0.0f))});
        textCollection.dynamicTexts.emplace("view", DynamicText{"view dir: (0.0, 0.0, 0.0)", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 768.0f - fontSize * 3, 0.0f))});

        std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        std::shared_ptr<Shader> universalShader = std::make_shared<Shader>("assets/shaders/universal.vert", "assets/shaders/universal.frag");
        std::shared_ptr<Shader> textureShader = std::make_shared<Shader>("assets/shaders/texture.vert", "assets/shaders/texture.frag");
        std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");

        std::shared_ptr<Shader> textShader = std::make_shared<Shader>("assets/shaders/text.vert", "assets/shaders/text.frag");

        textCollection.shader = textShader;

        std::vector<std::shared_ptr<Shader>> shaders { shader, universalShader, textureShader, lightingShader };

        
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
            material.use(universalShader);

            plane.draw(lightingShader);

            lightCube.draw(shader);

            object.draw(uniforms);

            texture.use();
            cube.draw(lightingShader);
            rect.draw(textureShader);
            cylinder.draw(lightingShader);
            sphere.draw(universalShader);

            if (renderContext->drawDebugHud) {
                glm::mat4 projection = glm::ortho(0.0f, (float) window.context->width, 0.0f, (float) window.context->height);
                textCollection.projection = projection;
                auto view = renderContext->camera.getView();
                textCollection.dynamicTexts["pos"].value = std::format("pos: ({:.2f},{:.2f},{:.2f})", uniforms.viewPos.x, uniforms.viewPos.y, uniforms.viewPos.z);
                textCollection.dynamicTexts["view"].value = std::format("view dir: ({:.2f},{:.2f},{:.2f})", view.x, view.y, view.z);

                // wireframe F3, cullMode F4, cameraMode F/O, uvs U, normals N, lightmode L+123
                std::string debugText = "debug modes:";
                if (renderContext->drawWireframe) debugText += " wireframe";
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
                if (renderContext->lightMode.x == 1.0f) debugText += " ambient";
                if (renderContext->lightMode.y == 1.0f) debugText += " diffuse";
                if (renderContext->lightMode.z == 1.0f) debugText += " specular";
                textCollection.dynamicTexts["modes"].value = debugText;


                textCollection.draw();
            }

            // TODO: This will need to go elsewhere i think
            glfwSwapBuffers(window.m_window);
        }
    }

    return EXIT_SUCCESS;
}

