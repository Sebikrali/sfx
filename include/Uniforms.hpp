#pragma once

#include "pch.h"
#include "Light.hpp"

struct RenderUniforms {
    glm::mat4 viewProj;
    glm::vec3 viewPos;
    glm::vec3 lightMode;
    PointLight pointLight;
};

