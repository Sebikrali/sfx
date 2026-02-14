#pragma once

#include "pch.h"
#include "Light.hpp"

struct RenderUniforms {
    glm::mat4 viewProj;
    glm::vec3 viewPos;
    glm::vec4 lightMode;
    glm::vec3 drawNormalsUVs;
    glm::vec2 hideMaterialTexture;
    SpotLight flashLight;
};

