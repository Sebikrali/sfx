#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 viewProj;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragUV;

void main() {
    gl_Position = viewProj * model * vec4(pos, 1.0);
    fragPos = vec3(model * vec4(pos, 1.0));
    fragNormal = normal;
    fragUV = uv;
}
