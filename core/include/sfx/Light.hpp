#pragma once

#include "pch.h"

// TODO: Add PBR/realistic lights where color is split up into each coefficient, meaning ambient, etc. are all a glm::vec3

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

