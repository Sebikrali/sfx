#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 dirLightViewProj;
uniform mat4 pointLightViewProj;

out vec2 fragUV;
out vec3 fragNormal;
out vec4 fragLightPosDir;
out vec4 fragLightPosPoint;

void main() {
    vec4 worldPos = model * vec4(pos, 1.0);
    gl_Position = viewProj * worldPos;
    fragUV = uv;
    fragNormal = mat3(model) * normal;

    fragLightPosDir = dirLightViewProj * worldPos;
    fragLightPosPoint = pointLightViewProj * worldPos;
}
