#version 450 core

in vec2 out_uv;

uniform sampler2D Texture;

uniform vec3 drawNormalsUVs;

out vec4 out_color;

void main() {
    if (drawNormalsUVs.y == 1.0) {
        out_color = vec4(out_uv, 0.0, 1.0);
        return;
    }
    out_color = vec4(texture(Texture, out_uv).xyz, 1.0f);
}
