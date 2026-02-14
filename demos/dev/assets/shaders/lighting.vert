#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat3 normalMat;
uniform mat4 viewProj;

out vec3 out_pos;
out vec3 out_normal;

void main() {
    gl_Position = viewProj * model * vec4(pos, 1.0);
    out_pos = vec3(model * vec4(pos, 1.0));
    out_normal = normalMat * normal;
}
