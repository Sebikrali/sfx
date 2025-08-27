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

struct Material {
    vec4 coefficients;
    vec3 color;
};
uniform Material material;

uniform sampler2D Texture;

uniform vec4 lightMode;
uniform vec3 drawNormalsUVs;
uniform vec2 hideMaterialTexture;

out vec4 color;

void main() {
    if (drawNormalsUVs.x == 1.0) {
        color = vec4(normalize(fragNormal), 1.0);
        return;
    } else if (drawNormalsUVs.y == 1.0) {
        color = vec4(fragUV, 0.0, 1.0);
        return;
    }

    float mixFactor = 0.5;
    vec4 matCoeffs = material.coefficients;
    vec3 matColor = material.color;
    if (hideMaterialTexture.x == 1.0) {
        matCoeffs = vec4(1.0, 1.0, 0.0, 0.0);
        matColor = vec3(1.0);
        mixFactor = 1.0;
    }

    vec3 texColor = texture(Texture, fragUV).xyz;
    if (hideMaterialTexture.y == 1.0) {
        texColor = vec3(1.0);
        mixFactor = 0.0;
    }

    vec3 fragColor = mix(matColor, texColor, mixFactor);

    if (lightMode.w == 0.0) {
        color = vec4(fragColor, 1.0);
        return;
    }

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = pointLight.pos - fragPos;
    float dist = length(lightDir);
    float attenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * dist + pointLight.attenuation.z * (dist * dist));
    lightDir = normalize(lightDir);

    vec3 ambient = pointLight.color * matCoeffs.x * attenuation;

    vec3 diffuse = pointLight.color * (matCoeffs.y * max(dot(lightDir, norm), 0.0)) * attenuation;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matCoeffs.w);
    vec3 specular = pointLight.color * matCoeffs.z * spec * attenuation;

    color = vec4((lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * fragColor, 1.0);
}
