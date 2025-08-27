#include "sfx/Material.hpp"

std::shared_ptr<Material> Material::Default() {
    return std::make_shared<Material>(glm::vec4{0.2f, 0.7f, 0.3f, 32.0f}, glm::vec3(0.0f));
}

void Material::use(std::shared_ptr<Shader> shader) const {
    shader->use();
    shader->setUniform("material.coefficients", m_coefficients);
    shader->setUniform("material.color", m_color);
}

