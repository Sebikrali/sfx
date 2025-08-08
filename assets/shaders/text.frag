#version 450 core

in vec2 fragUV;

uniform sampler2D Texture;

out vec4 color;

void main() {
    color = vec4(0.0, 0.0, 0.0, texture(Texture, fragUV).r);
}
