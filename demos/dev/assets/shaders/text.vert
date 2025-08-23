#version 450 core

layout(location = 0) in vec2 pos;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 viewProj;

out vec2 fragUV;

void main() {
    gl_Position = viewProj * model * vec4(pos, 0.0, 1.0);
    fragUV = uv;
}
