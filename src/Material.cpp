#include "Material.hpp"

void Material::use(std::shared_ptr<Shader> shader) const {
    shader->use();
    // NOTE: Maybe i need to add a name field to substitute "material"
    shader->setUniform("material.ambient", m_ambient);
    shader->setUniform("material.diffuse", m_diffuse);
    shader->setUniform("material.specular", m_specular);
    shader->setUniform("material.shininess", m_shininess);

    // NOTE: This is undefined if m_color isn't initialized, could lead to weird behaviour
    shader->setUniform("u_color", m_color);
}

