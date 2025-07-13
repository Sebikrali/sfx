#include "Object.hpp"

Object Object::Default() {
    return Object(
        "DefaultObject",
        Mesh::Default(),
        Shader::Default(),
        Material::Default(),
        Texture::Default()
    );
}

void Object::draw(const RenderUniforms& u) const {
    shader->use();
    shader->setUniform("viewProj", u.viewProj);
    shader->setUniform("viewPos", u.viewPos);
    shader->setUniform("lightMode", u.lightMode);
    shader->setUniform("drawNormalsUVs", u.drawNormalsUVs);
    shader->setUniform("pointLight", u.pointLight);
    material->use(shader);
    texture->use();
    mesh->draw(shader);
}

