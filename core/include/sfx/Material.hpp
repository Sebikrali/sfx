#pragma once

#include "pch.h"
#include "Shader.hpp"

struct Material {
    Material() = delete;
    Material(glm::vec4 coefficients, glm::vec3 color) : m_coefficients(coefficients), m_color(color) {}
    ~Material() = default;

    static std::shared_ptr<Material> Default();

    void use(std::shared_ptr<Shader> shader) const;

private:
    /*!
     * The material's coefficients (x = ambient, y = diffuse, z = specular, w = shininess)
     */
    glm::vec4 m_coefficients;
    glm::vec3 m_color;
};

// TODO: Add PBR/realistic material where ambient, etc. are a glm::vec3
