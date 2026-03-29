#version 460

in vec2 fragUV;
in vec3 fragNormal;
in vec4 fragLightPosDir;
in vec4 fragLightPosPoint;

uniform vec3 dirLightDir;
uniform vec3 pointLightDir;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2DShadow shadowMap;
layout(binding = 2) uniform sampler2DShadow perspectiveShadowMap;

out vec4 outColor;

float shadowcalc(sampler2DShadow map, vec4 pos, vec3 lightDir) {
    vec3 ndc = pos.xyz / pos.w;
    ndc = ndc * 0.5 + 0.5;
    if (ndc.z > 1.0) {
        return 1.0;
    }

    // the steeper the bigger the bias should be
    float bias = max(0.15 * (1.0 - dot(fragNormal, -lightDir)), 0.005);

    float shadowSum = 0.0;
    vec2 texelSize = 1.0 / textureSize(map, 0);
    for (int y = -1; y < 1; y++) {
        for (int x = -1; x < 1; x++) {
            vec2 uv = vec2(ndc.x + x * texelSize.x, ndc.y + y * texelSize.y);
            // NOTE: This uses the sampler2DShadow to do the depth comparison, returns 0 or 1
            shadowSum += texture(map, vec3(uv, ndc.z - bias));
        }
    }
    shadowSum /= 18.0;
    return 0.5 + shadowSum;
}

void main() {
    // NOTE: When adding correct lighting, this should happen on a basis of per light (aka the shadow should only contribute to the corresponding light factor)

    float shadow = shadowcalc(shadowMap, fragLightPosDir, dirLightDir);
    shadow *= shadowcalc(perspectiveShadowMap, fragLightPosPoint, pointLightDir);
    outColor = vec4(texture(tex, fragUV).xyz * min(shadow, 1.0), 1.0);
}
