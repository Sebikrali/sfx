#version 450 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

out vec4 out_color;

void main() {
    out_color = vec4(material.ambient, 1.0f);
}
