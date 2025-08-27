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

struct Material {
    vec4 coefficients;
    vec3 color;
};
uniform Material material;

uniform vec4 lightMode;
uniform vec3 drawNormalsUVs;

out vec4 out_color;

void main() {
    if (drawNormalsUVs.x == 1.0) {
        out_color = vec4(normalize(out_normal), 1.0);
        return;
    }

    if (lightMode.w == 0.0) {
        out_color = vec4(material.color, 1.0);
        return;
    }

    vec3 norm = normalize(out_normal);
    vec3 lightDir = pointLight.pos - out_pos;
    float dist = length(lightDir);
    float attenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * dist + pointLight.attenuation.z * (dist * dist));
    lightDir = normalize(lightDir);

    vec3 ambient = pointLight.color * material.coefficients.x * attenuation;

    vec3 diffuse = pointLight.color * (material.coefficients.y * max(dot(lightDir, norm), 0.0)) * attenuation;

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.coefficients.w);
    vec3 specular = pointLight.color * material.coefficients.z * spec * attenuation;

    out_color = vec4((lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * material.color, 1.0);
}
