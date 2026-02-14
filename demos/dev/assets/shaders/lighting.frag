#version 460 core

in vec3 out_pos;
in vec3 out_normal;

uniform vec3 viewPos;

struct PointLight {
    vec4 pos;
    vec4 color;
    vec4 attenuation;
};
struct DirLight {
    vec4 direction;
    vec4 color;
};
struct SpotLight {
    vec4 pos;
    vec4 direction;
    vec4 color_and_cutoff; // .w = cutoff angle in radians
};
layout(std140, binding = BINDING_POINT_LIGHTS) uniform Lights {
    PointLight pointLights[NUM_POINT_LIGHTS];
    DirLight dirLights[NUM_DIR_LIGHTS];
    SpotLight spotLights[NUM_SPOT_LIGHTS];
};

struct Material {
    vec4 coefficients;
    vec3 color;
};
uniform Material material;

uniform SpotLight flashlight;

uniform vec4 lightMode;
uniform vec3 drawNormalsUVs;

out vec4 out_color;

vec3 calc_pointlight(PointLight light, vec3 n) {
    vec3 lightDir = light.pos.xyz - out_pos;
    float dist = length(lightDir);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
    lightDir = normalize(lightDir);

    vec3 lightColor = light.color.xyz;
    vec3 ambient = lightColor * material.coefficients.x * attenuation;

    vec3 diffuse = lightColor * (material.coefficients.y * max(dot(lightDir, n), 0.0)) * attenuation;

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.coefficients.w);
    vec3 specular = lightColor * material.coefficients.z * spec * attenuation;

    return (lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * material.color;
}

vec3 calc_dirlight(DirLight light, vec3 n) {
    vec3 lightDir = normalize(light.direction.xyz);
    vec3 lightColor = light.color.xyz;

    vec3 ambient = lightColor * material.coefficients.x;

    vec3 diffuse = lightColor * (material.coefficients.y * max(dot(lightDir, n), 0.0));

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.coefficients.w);
    vec3 specular = lightColor * material.coefficients.z * spec;

    return (lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * material.color;
}

vec3 calc_spotlight(SpotLight light, vec3 n) {
    vec3 lightColor = light.color_and_cutoff.xyz;

    vec3 ambient = lightColor * material.coefficients.x;

    vec3 lightDir = normalize(light.pos.xyz - out_pos);
    vec3 spotDir = normalize(-light.direction.xyz);

    float theta = dot(lightDir, spotDir);
    if (theta < light.color_and_cutoff.w) {
        return lightMode.x * ambient * material.color;
    }

    vec3 diffuse = lightColor * (material.coefficients.y * max(dot(lightDir, n), 0.0));

    vec3 viewDir = normalize(viewPos - out_pos);
    vec3 reflectDir = reflect(-lightDir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.coefficients.w);
    vec3 specular = lightColor * material.coefficients.z * spec;

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

    vec3 color = vec3(0.0);
    // We have a dummy at i=0 of each lighting array, because arrays of dimension 0 are not allowed
    for (uint i = 1; i < NUM_POINT_LIGHTS; i++) {
        color += calc_pointlight(pointLights[i], norm);
    }
    for (uint i = 1; i < NUM_DIR_LIGHTS; i++) {
        color += calc_dirlight(dirLights[i], norm);
    }
    for (uint i = 1; i < NUM_SPOT_LIGHTS; i++) {
        color += calc_spotlight(spotLights[i], norm);
    }

    color += calc_spotlight(flashlight, norm);
    out_color = vec4(color, 1.0);
}
