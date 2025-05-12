#version 450 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 viewProj;

void main() {
    gl_Position = viewProj * model * vec4(pos, 1.0);
}
