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
    if (!shader) 
        throw std::runtime_error("Error: Object '" + name + "' has no shader");
    if (!mesh) 
        throw std::runtime_error("Error: Object '" + name + "' has no mesh");

    shader->use();
    shader->setUniform("viewProj", u.viewProj);
    shader->setUniform("viewPos", u.viewPos);
    shader->setUniform("lightMode", u.lightMode);
    shader->setUniform("drawNormalsUVs", u.drawNormalsUVs);
    shader->setUniform("pointLight", u.pointLight);
    if (material) {
        material->use(shader);
    }
    if (texture) {
        texture->use();
    }
    mesh->draw(shader);
}

