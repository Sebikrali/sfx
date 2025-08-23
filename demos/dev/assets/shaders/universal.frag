#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUV;

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

uniform sampler2D Texture;

uniform vec3 lightMode;
uniform vec3 drawNormalsUVs;

out vec4 color;

void main() {
    if (drawNormalsUVs.x == 1.0) {
        color = vec4(normalize(fragNormal), 1.0);
        return;
    } else if (drawNormalsUVs.y == 1.0) {
        color = vec4(fragUV, 0.0, 1.0);
        return;
    }

    vec3 fragColor = mix(texture(Texture, fragUV).xyz, u_color, 0.5);

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = pointLight.pos - fragPos;
    float dist = length(lightDir);
    float attenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * dist + pointLight.attenuation.z * (dist * dist));
    lightDir = normalize(lightDir);

    vec3 ambient = pointLight.color * material.x * attenuation;

    vec3 diffuse = pointLight.color * (material.y * max(dot(lightDir, norm), 0.0)) * attenuation;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = pointLight.color * material.z * spec * attenuation;

    color = vec4((lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * fragColor, 1.0);
}
