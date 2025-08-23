#pragma once

#include "pch.h"
#include "Shader.hpp"

struct Material {
    Material() = delete;
    Material(glm::vec3 coefficients, float shininess, glm::vec3 color) : m_coefficients(coefficients), m_shininess(shininess), m_color(color) {}
    ~Material() = default;

    static std::shared_ptr<Material> Default();

    void use(std::shared_ptr<Shader> shader) const;

private:
    /*!
     * The material's coefficients (x = ambient, y = diffuse, z = specular)
     */
    glm::vec3 m_coefficients; // NOTE: Maybe make this a vec4 with the shininess (if it works)
    float m_shininess;
    glm::vec3 m_color;
};

// TODO: Add PBR/realistic material where ambient, etc. are a glm::vec3
