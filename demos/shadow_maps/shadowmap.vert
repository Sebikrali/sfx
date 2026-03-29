#version 460

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 lightViewProj;

void main() {
    gl_Position = lightViewProj * model * vec4(pos, 1.0);
}
