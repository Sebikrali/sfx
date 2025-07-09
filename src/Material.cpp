#include "Material.hpp"

void Material::use(std::shared_ptr<Shader> shader) const {
    shader->use();
    shader->setUniform("material", m_coefficients);
    shader->setUniform("shininess", m_shininess);
    shader->setUniform("u_color", m_color);
}

