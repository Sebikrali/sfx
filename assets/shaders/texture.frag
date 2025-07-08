#version 450 core

in vec2 out_uv;

uniform sampler2D Texture;

out vec4 out_color;

void main() {
    out_color = vec4(texture(Texture, out_uv).xyz, 1.0f);
}
