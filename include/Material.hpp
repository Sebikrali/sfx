#pragma once

#include "pch.h"
#include "Shader.hpp"

class Material {
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;

    glm::vec3 m_color;

public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {}
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 color) : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess), m_color(color) {}

    void use(std::shared_ptr<Shader> shader) const;
};
