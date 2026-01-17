#pragma once

#include "pch.h"

// TODO: Add PBR/realistic lights where color is split up into each coefficient, meaning ambient, etc. are all a glm::vec3

struct PointLight {
    glm::vec3 pos;
    glm::vec3 color;
    /*!
     * Attenuation: (x = constant, y = linear, z = quadratic)
     */
    glm::vec3 attenuation;
};

struct DirLight {
    glm::vec3 direction;
    glm::vec3 color;
};

struct SpotLight {
    glm::vec3 pos;
    glm::vec3 direction;
    float cutoff; // cutoff angle in radians
    glm::vec3 color;
};

