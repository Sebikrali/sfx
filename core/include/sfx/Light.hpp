#pragma once

#include "pch.h"

// Standard, good for rooms
constexpr glm::vec4 ATT_STANDARD = glm::vec4(1.0f, 0.09f, 0.032f, 0.0f);
// Very short range, sharp fade, use for example for spotlights
constexpr glm::vec4 ATT_DIM = glm::vec4(1.0f, 0.7f, 18.0f, 0.0f);
// Long range, soft fade, use for example for streetlamps
constexpr glm::vec4 ATT_BRIGHT = glm::vec4(1.0f, 0.02f, 0.001f, 0.0f);

inline glm::vec4 create_att_for_range(float radius) {
    return glm::vec4(1.0f, 10.0f / radius, 100.0f / (radius * radius), 0.0f);
}

struct PointLight {
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec4 attenuation; // (x = constant, y = linear, z = quadratic)
};

struct DirLight {
    glm::vec4 direction;
    glm::vec4 color;
};

struct SpotLight {
    glm::vec4 pos;
    glm::vec4 direction;
    glm::vec4 color;  // .w = cutoff angle in radians
};

