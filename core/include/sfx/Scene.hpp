#pragma once

#include "sfx/Light.hpp"
#include "sfx/Shader.hpp"

struct Scene {

    GLuint BINDING_POINT_STATIC_LIGHTS = 0;
    GLuint BINDING_POINT_DYNAMIC_LIGHTS = 1;

    // To add resources: Meshes, Shader(OpenGL), Materials, Textures, etc.

    std::vector<std::shared_ptr<Shader>> shader;

    std::vector<ShaderDefine> lightingDefines;

    // Static Lights
    std::vector<PointLight> pointLights;
    std::vector<DirLight> dirLights;
    std::vector<SpotLight> spotLights;

    // Dynamic Light
    SpotLight flashLight;

    Scene();
    void init();

    void draw();

private:

    GLuint staticLightsBuffer;
    // GLuint dynamicLightsBuffer;
};
