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

struct DirLight {
    vec3 direction;
    vec3 color;
};
uniform DirLight dirLight;

struct SpotLight {
    vec3 pos;
    vec3 direction;
    float cutoff;
    vec3 color;
};
uniform SpotLight spotLight;

struct Material {
    vec4 coefficients;
    vec3 color;
};
uniform Material material;

uniform vec4 lightMode;
uniform vec3 drawNormalsUVs;

out vec4 out_color;

// TODO: Think of a better name for these

vec3 calc_pointlight(PointLight light, vec3 n) {
    vec3 lightDir = light.pos - out_pos;
    float dist = length(lightDir);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
    lightDir = normalize(lightDir);

    vec3 ambient = light.color * material.coefficients.x * attenuation;

    vec3 diffuse = light.color * (material.coefficients.y * max(dot(lightDir, n), 0.0)) * attenuation;

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.coefficients.w);
    vec3 specular = light.color * material.coefficients.z * spec * attenuation;

    return (lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * material.color;
}

vec3 calc_dirlight(DirLight light, vec3 n) {
    vec3 lightDir = normalize(light.direction);

    vec3 ambient = light.color * material.coefficients.x;

    vec3 diffuse = light.color * (material.coefficients.y * max(dot(lightDir, n), 0.0));

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.coefficients.w);
    vec3 specular = light.color * material.coefficients.z * spec;

    return (lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * material.color;
}

vec3 calc_spotlight(SpotLight light, vec3 n) {
    vec3 ambient = light.color * material.coefficients.x;

    vec3 lightDir = normalize(light.pos - out_pos);
    vec3 spotDir = normalize(-light.direction);

    float theta = dot(lightDir, spotDir);
    if (theta < light.cutoff) {
        return lightMode.x * ambient * material.color;
    }

    vec3 diffuse = light.color * (material.coefficients.y * max(dot(lightDir, n), 0.0));

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.coefficients.w);
    vec3 specular = light.color * material.coefficients.z * spec;

    return (lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * material.color;
}

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

    // out_color = vec4(calc_pointlight(pointLight, norm), 1.0);
    // out_color = vec4(calc_dirlight(dirLight, norm), 1.0);
    out_color = vec4(calc_spotlight(spotLight, norm), 1.0);
}
