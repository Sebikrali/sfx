#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "sfx/pch.h"
#include "sfx/Scene.hpp"
#include "sfx/Debug.hpp"
#include "sfx/Window.hpp"
#include "sfx/Timer.hpp"
#include "sfx/Object.hpp"

const unsigned int shadowWidth = 1024;
const unsigned int shadowHeight = 1024;

// TODO: 
// - make multiple object shader that have different variations of artifacts and fixes, then let me switch between them at runtime with ImGui

struct Framebuffer {
    unsigned int fbo;
    unsigned int depthAttachment;
};

Framebuffer create_depthmap(bool use_hardware_lookup) {
    Framebuffer f;
    glGenFramebuffers(1, &f.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, f.fbo);

    glCreateTextures(GL_TEXTURE_2D, 1, &f.depthAttachment);
    glTextureStorage2D(f.depthAttachment, 1, GL_DEPTH_COMPONENT32F, shadowWidth, shadowHeight);

    glTextureParameteri(f.depthAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(f.depthAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(f.depthAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTextureParameteri(f.depthAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTextureParameterfv(f.depthAttachment, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (use_hardware_lookup) {
        glTextureParameteri(f.depthAttachment, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTextureParameteri(f.depthAttachment, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, f.depthAttachment, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ASSERT(false, "Couldn't generate framebuffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return f;
} 

struct ScreenQuad {
    unsigned int vao;
    unsigned int vbo, ebo;
};

/**
 * @param bounds - x = right, y = top, z = left, w = bottom.
 */
ScreenQuad create_screen_quad(glm::vec4 bounds) {
    ScreenQuad sq;
    float vertices[] = {
        // pos        uvs
        bounds.x, bounds.y,   1.0f, 1.0f,
        bounds.z, bounds.y,   0.0f, 1.0f,
        bounds.z, bounds.w,   0.0f, 0.0f,
        bounds.x, bounds.w,   1.0f, 0.0f,
    };

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0
    };

    glCreateVertexArrays(1, &sq.vao); 

    glCreateBuffers(1, &sq.vbo);
    glNamedBufferData(sq.vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(sq.vao, 0, sq.vbo, 0, sizeof(float) * 4);
    glEnableVertexArrayAttrib(sq.vao, 0);
    glVertexArrayAttribBinding(sq.vao, 0, 0);
    glVertexArrayAttribFormat(sq.vao, 0, 2, GL_FLOAT, GL_FALSE, 0); 
    glEnableVertexArrayAttrib(sq.vao, 1);
    glVertexArrayAttribBinding(sq.vao, 1, 0);
    glVertexArrayAttribFormat(sq.vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2);

    glCreateBuffers(1, &sq.ebo);
    glNamedBufferData(sq.ebo, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(sq.vao, sq.ebo);

    return sq;
}

int main(int argc, char* argv[]) {
    bool use_hardware_lookup = false;
    if (argc > 1) {
        use_hardware_lookup = true;
    }

    Window window;
    auto wCtx = window.context;
    auto rCtx = window.renderContext;
    rCtx->camera.setPos({ 0.0f, 1.0f, 3.0f });

    window.setupDebugHud("");

    glm::vec3 dirLightDir(-3.0f, -4.0f, 0.0f);
    glm::vec3 pointLightPos(10.0f, 4.0f, 0.0f);

    Scene scene;
    scene.dirLights.emplace_back( glm::vec4(dirLightDir, .0f), glm::vec4{1.0f, 1.0f, 1.0f, 1.0f} );
    // NOTE: These are not actually used right now
    scene.pointLights.emplace_back( glm::vec4(pointLightPos, .0f), glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}, glm::vec4{1.0f, 0.09, 0.032, .0f});
    scene.init();

    std::shared_ptr<Shader> basicShader;
    if (use_hardware_lookup) {
        basicShader = std::make_shared<Shader>("main.vert", "main_with_hardware_lookup.frag");
    } else {
        basicShader = std::make_shared<Shader>("main.vert", "main.frag");
    }

    auto lightShader = std::make_shared<Shader>("light_visualization.vert", "light_visualization.frag");
    std::vector<Object> lightCubes;
    auto& dirLCube = lightCubes.emplace_back();
    dirLCube.mesh = Mesh::Default();
    dirLCube.mesh->m_modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), -dirLightDir), glm::vec3(.2f));
    dirLCube.shader = lightShader;
    auto& posLCube = lightCubes.emplace_back();
    posLCube.mesh = Mesh::Default();
    posLCube.mesh->m_modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), pointLightPos), glm::vec3(.2f));
    posLCube.shader = lightShader;


    std::vector<Object> meshes;
    auto& cube1 = meshes.emplace_back();
    cube1.mesh = Mesh::Default();
    cube1.mesh->m_modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), (std::numbers::pi_v<float> / 4.0f), glm::vec3(.0f, 1.0f, .0f));
    cube1.shader = basicShader;
    cube1.texture = std::make_shared<Texture>("container.jpg");

    auto& cube2 = meshes.emplace_back();
    cube2.mesh = Mesh::Default();
    cube2.mesh->m_modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pointLightPos.x, 1.0f, pointLightPos.z)) * glm::rotate(glm::mat4(1.0f), (std::numbers::pi_v<float> / 4.0f), glm::vec3(.0f, 1.0f, .0f));
    cube2.shader = basicShader;
    cube2.texture = std::make_shared<Texture>("container.jpg");

    auto& plane = meshes.emplace_back();
    plane.mesh = std::make_shared<Mesh>(MeshData::Plane(100.0f), glm::mat4(1.0f));
    plane.shader = basicShader;
    plane.texture = std::make_shared<Texture>("default.png");


    glEnable(GL_DEPTH_TEST);

    Framebuffer fbo1 = create_depthmap(use_hardware_lookup);
    Framebuffer fbo2 = create_depthmap(use_hardware_lookup);

    std::shared_ptr<Shader> depthShader = std::make_shared<Shader>("shadowmap.vert", "shadowmap.frag");

    float near_plane = 1.0f, far_plane = 10.0f;
    glm::mat4 dirLightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 dirLightView = glm::lookAt(-dirLightDir, glm::vec3(.0f, .0f, .0f), glm::vec3(.0f, 1.0f, .0f));
    glm::mat4 dirLightTransform = dirLightProj * dirLightView;

    glm::mat4 pointLightProj = glm::perspective(glm::radians(45.0f), (float) wCtx->width / (float) wCtx->height, near_plane, far_plane);
    glm::mat4 pointLightView = glm::lookAt(pointLightPos, pointLightPos - glm::vec3(.5f, pointLightPos.y, .0f), glm::vec3(.0f, 1.0f, .0f)); 
    glm::mat4 pointLightTransform = pointLightProj * pointLightView;

    basicShader->use();
    basicShader->setUniform("dirLightViewProj", dirLightTransform);
    basicShader->setUniform("dirLightDir", glm::vec3(scene.dirLights[1].direction));
    basicShader->setUniform("pointLightViewProj", pointLightTransform);
    basicShader->setUniform("pointLightDir", -pointLightPos);

    // Create a simple 2d quad and render the texture on it
    ScreenQuad sq1 = create_screen_quad(glm::vec4(1.0f, 1.0f, 0.5f, 0.5f));
    ScreenQuad sq2 = create_screen_quad(glm::vec4(1.0f, 0.45f, 0.5f, -0.05f));

    auto overlayShader = std::make_shared<Shader>("overlay.vert", "overlay.frag");
    overlayShader->use();
    overlayShader->setUniform("near_plane", near_plane);
    overlayShader->setUniform("far_plane", far_plane);

    Timer frame(true);
    while (!window.shouldClose()) {
        if (window.poll()) {
            continue;
        }
        float dt = frame.stopAndRestart();
        window.update(dt);

        RenderUniforms u = {
            .viewProj = rCtx->camera.getViewProjMatrix(),
            .viewPos = rCtx->camera.getPos(),
            .lightMode = rCtx->lightMode,
            .drawNormalsUVs = rCtx->drawNormalsUVs,
        };

        // Draw the scene into the dir light depthmap
        glBindFramebuffer(GL_FRAMEBUFFER, fbo1.fbo);
        glViewport(0, 0, shadowWidth, shadowHeight);
        glClear(GL_DEPTH_BUFFER_BIT);

        // NOTE: Use Back-Side Rendering to avoid self-shadowing
        glCullFace(GL_FRONT);

        depthShader->use();
        depthShader->setUniform("lightViewProj", dirLightTransform);
        for (auto& m : meshes) {
            m.shader = depthShader;
            m.draw(u);
        }

        // Draw the scene into the point light depthmap
        glBindFramebuffer(GL_FRAMEBUFFER, fbo2.fbo);
        glClear(GL_DEPTH_BUFFER_BIT);

        depthShader->use();
        depthShader->setUniform("lightViewProj", pointLightTransform);
        for (auto& m : meshes) {
            m.draw(u);
        }

        // Draw the scene normally + shadows
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, wCtx->width, wCtx->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glCullFace(GL_BACK);

        glBindTextureUnit(1, fbo1.depthAttachment);
        glBindTextureUnit(2, fbo2.depthAttachment);
        for (auto& m : meshes) {
            m.shader = basicShader;
            m.draw(u);
        }

        for (const auto& lightCube : lightCubes) {
            lightCube.draw(u);
        }

        // Draw the depthmap to a overlay
        overlayShader->use();
        overlayShader->setUniform("perspective", 0);

        glBindTextureUnit(0, fbo1.depthAttachment);
        glBindVertexArray(sq1.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        overlayShader->setUniform("perspective", 1);
        glBindTextureUnit(0, fbo2.depthAttachment);
        glBindVertexArray(sq2.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.drawDebugHud();

        glfwSwapBuffers(window.m_window);
    }
    
    glDeleteFramebuffers(1, &fbo1.fbo);  
    glDeleteFramebuffers(1, &fbo2.fbo);  
}
