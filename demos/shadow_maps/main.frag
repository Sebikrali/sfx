#version 460

in vec2 fragUV;
in vec3 fragNormal;
in vec4 fragLightPosDir;
in vec4 fragLightPosPoint;

uniform vec3 dirLightDir;
uniform vec3 pointLightDir;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D shadowMap;
layout(binding = 2) uniform sampler2D perspectiveShadowMap;

out vec4 outColor;

// Simplest with no fixes
float shadowcalc(vec4 pos) {
    vec3 ndc = pos.xyz / pos.w;
    ndc = ndc * 0.5 + 0.5;

    float closest_depth = texture(shadowMap, ndc.xy).r;
    float curr_depth = ndc.z;

    if (closest_depth < curr_depth) {
        return 0.0;
    }
    return 1.0;
}

// With depth bias for shadow acne / self-shadowing
float shadowcalc_with_depth_bias(vec4 pos, vec3 lightDir) {
    vec3 ndc = pos.xyz / pos.w;
    ndc = ndc * 0.5 + 0.5;

    // the steeper the bigger the bias should be
    float bias = max(0.15 * (1.0 - dot(fragNormal, -lightDir)), 0.005);

    float closest_depth = texture(shadowMap, ndc.xy).r;
    float curr_depth = ndc.z;

    if (closest_depth < (curr_depth - bias)) {
        return 0.0;
    }
    return 1.0;
}

float shadowcalc_with_oversampling(vec4 pos) {
    vec3 ndc = pos.xyz / pos.w;
    ndc = ndc * 0.5 + 0.5;
    if (ndc.z > 1.0) {
        return 1.0;
    }

    float closest_depth = texture(shadowMap, ndc.xy).r;
    float curr_depth = ndc.z;

    if (closest_depth < curr_depth) {
        return 0.0;
    }
    return 1.0;
}

float shadowcalc_with_pcf(vec4 pos) {
    vec3 ndc = pos.xyz / pos.w;
    ndc = ndc * 0.5 + 0.5;
    float curr_depth = ndc.z;

    float shadowSum = 0.0;
    vec2 texelSize = textureSize(shadowMap, 0);
    for (int y = -1; y < 1; y++) {
        for (int x = -1; x < 1; x++) {
            float closest_depth = texture(shadowMap, ndc.xy + vec2(x, y) * texelSize).r;
            shadowSum += closest_depth < curr_depth ? 1.0 : 0.0;
        }
    }
    shadowSum /= 9.0;
    return shadowSum;
}

float shadowcalc_final(sampler2D map, vec4 pos, vec3 lightDir) {
    vec3 ndc = pos.xyz / pos.w;
    ndc = ndc * 0.5 + 0.5;
    if (ndc.z > 1.0) {
        return 1.0;
    }
    float curr_depth = ndc.z;

    // the steeper the bigger the bias should be
    float bias = max(0.15 * (1.0 - dot(fragNormal, -lightDir)), 0.005);

    float shadowSum = 0.0;
    vec2 texelSize = 1.0 / textureSize(map, 0);
    for (int y = -1; y < 1; y++) {
        for (int x = -1; x < 1; x++) {
            float closest_depth = texture(map, ndc.xy + vec2(x, y) * texelSize).r;
            shadowSum += closest_depth < (curr_depth - bias) ? 1.0 : 0.0;
        }
    }
    shadowSum /= 9.0;
    return 1.0 - shadowSum;
}

void main() {
    // NOTE: When adding correct lighting, this should happen on a basis of per light (aka the shadow should only contribute to the corresponding light factor)

    float shadow = shadowcalc_final(shadowMap, fragLightPosDir, dirLightDir);
    shadow *= shadowcalc_final(perspectiveShadowMap, fragLightPosPoint, pointLightDir);
    outColor = vec4(texture(tex, fragUV).xyz * min(shadow, 1.0), 1.0);
}
