#include "sfx/Scene.hpp"
#include "sfx/Light.hpp"
#include "sfx/Shader.hpp"
#include "sfx/pch.h"

Scene::Scene() {
    // We add a dummy at i=0 of each lighting array, because arrays of dimension 0 are not allowed in GLSL and this way we can have for example 0 direction lights
    pointLights.emplace_back();
    dirLights.emplace_back();
    spotLights.emplace_back();
}

void Scene::init() {

    // ================== //
    //   Lighting setup   //
    // ================== //

    lightingDefines = {
        ShaderDefine{.type = FRAG, .name = "BINDING_POINT_LIGHTS", .value = static_cast<int32_t>(BINDING_POINT_STATIC_LIGHTS)},
        ShaderDefine{.type = FRAG, .name = "NUM_POINT_LIGHTS", .value = static_cast<int32_t>(pointLights.size())},
        ShaderDefine{.type = FRAG, .name = "NUM_DIR_LIGHTS", .value = static_cast<int32_t>(dirLights.size())},
        ShaderDefine{.type = FRAG, .name = "NUM_SPOT_LIGHTS", .value = static_cast<int32_t>(spotLights.size())},
    };

    // TODO: Maybe it would be good to have two types of structs: 1) for the c++ code and better readability and 2) one that has the better format for the shader

    // Create uniform buffer for static lights
    size_t pointSize = sizeof(PointLight) * pointLights.size();
    size_t dirSize = sizeof(DirLight) * dirLights.size();
    size_t spotSize = sizeof(SpotLight) * spotLights.size();
    glCreateBuffers(1, &staticLightsBuffer);
    glNamedBufferStorage(staticLightsBuffer, pointSize + dirSize + spotSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glNamedBufferSubData(staticLightsBuffer, 0, pointSize, pointLights.data());
    glNamedBufferSubData(staticLightsBuffer, pointSize, dirSize, dirLights.data());
    glNamedBufferSubData(staticLightsBuffer, pointSize + dirSize, spotSize, spotLights.data());

    glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT_STATIC_LIGHTS, staticLightsBuffer);

    // Create storage buffer for dynamic light

    // glCreateBuffers(1, &staticLightsBuffer);
    // glNamedBufferStorage(staticLightsBuffer, pointSize + dirSize + spotSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    //
    // glNamedBufferSubData(staticLightsBuffer, 0, pointSize, pointLights.data());
    // glNamedBufferSubData(staticLightsBuffer, pointSize, dirSize, dirLights.data());
    // glNamedBufferSubData(staticLightsBuffer, pointSize + dirSize, spotSize, spotLights.data());
    //
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BINDING_POINT_LIGHTS, staticLightsBuffer);

}


void Scene::draw() {
    for (const auto& s : shader) {
        s->use();
        if (s->resources & DYNAMIC_LIGHTS) {
            // TODO: Bind the uniform buffer or do it individually idk
            // s->setUniform("pointLight", uniforms.pointLight);
            // s->setUniform("dirLight", uniforms.dirLight);
            // s->setUniform("spotLight", uniforms.spotLight);
        }
    }
}
