#version 450

uniform float uStartDist;
uniform float uEndDist;

uniform float uNear;
uniform float uFar;

out vec4 color;

float LinearDepthFromZ(float z_buf) {
    // Convert back to NDC ([-1, 1] for OpenGL)
    float z_ndc = z_buf * 2.0 - 1.0;

    // Inverse of the perspective projection's depth equation:
    //   z_ndc = (f + n) / (f - n) + (2*f*n) / (f - n) * (1 / z_eye)
    // Solve for z_eye:
    return (2.0 * uNear * uFar) /
           (uFar + uNear - z_ndc * (uFar - uNear));
}

void main() {
    float z_eye = LinearDepthFromZ(gl_FragCoord.z);
    float depth_normalized = clamp((z_eye - uStartDist) / (uEndDist - uStartDist), 0.0, 1.0);
    vec3 depth_color = vec3(depth_normalized);
    color = vec4(1 - depth_color, 1.0);
}
