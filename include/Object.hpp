#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Uniforms.hpp"

struct Object {
    std::string name;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Material> material;
    std::shared_ptr<Texture> texture;

    Object() = default;
    Object(const std::string& name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, std::shared_ptr<Material> material, std::shared_ptr<Texture> texture) : name(name), mesh(mesh), shader(shader), material(material), texture(texture) {}
    ~Object() = default;

    static Object Default();

    void draw(const RenderUniforms& u) const;
};
