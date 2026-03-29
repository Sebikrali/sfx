#version 460

in vec2 fragUv;

layout(binding = 0) uniform sampler2D depthmap;

uniform int perspective;

uniform float near_plane;
uniform float far_plane;

out vec4 color;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
    float depth = texture(depthmap, fragUv).r;

    if (perspective == 0) {
        color = vec4(vec3(depth), 1.0);
    } else {
        color = vec4(vec3(LinearizeDepth(depth) / far_plane), 1.0);
    }
}

