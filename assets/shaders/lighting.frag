#version 450 core

in vec3 out_pos;
in vec3 out_normal;

uniform vec3 viewPos;

struct PointLight {
    vec3 pos;
    vec3 color;
    vec3 attenuation;
};
uniform PointLight pointLight;

uniform vec3 material;
uniform float shininess;
uniform vec3 u_color;

uniform vec3 lightMode;

out vec4 out_color;

void main() {
    vec3 norm = normalize(out_normal);
    vec3 lightDir = pointLight.pos - out_pos;
    float dist = length(lightDir);
    float attenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * dist + pointLight.attenuation.z * (dist * dist));
    lightDir = normalize(lightDir);

    vec3 ambient = pointLight.color * material.x * attenuation;

    vec3 diffuse = pointLight.color * (material.y * max(dot(lightDir, norm), 0.0)) * attenuation;

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = pointLight.color * material.z * spec * attenuation;

    out_color = vec4((lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * u_color, 1.0);
}
