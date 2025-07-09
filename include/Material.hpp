#pragma once

#include "pch.h"
#include "Shader.hpp"

class Material {
    /*!
     * The material's coefficients (x = ambient, y = diffuse, z = specular)
     */
    glm::vec3 m_coefficients; // NOTE: Maybe make this a vec4 with the shininess (if it works)
    float m_shininess;
    glm::vec3 m_color;

public:
    Material(glm::vec3 coefficients, float shininess, glm::vec3 color) : m_coefficients(coefficients), m_shininess(shininess), m_color(color) {}

    void use(std::shared_ptr<Shader> shader) const;
};

// TODO: Add PBR/realistic material where ambient, etc. are a glm::vec3
