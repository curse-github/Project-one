#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragNormal;

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    mat4 projectionView;
    mat3 transform;
} push;

void main() {
    outColor = vec4(fragColor, 1.0);
}