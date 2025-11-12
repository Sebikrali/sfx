#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 viewProj;

void main() {
    gl_Position = viewProj * model * vec4(pos, 1.0);
}
