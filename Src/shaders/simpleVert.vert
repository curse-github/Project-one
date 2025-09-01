#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec3 vertColor;
layout (location = 1) out vec3 vertNormal;

layout(push_constant) uniform Push {
    mat4 projectionView;
    mat4 transform;
} push;

void main() {
    gl_Position = push.projectionView * push.transform * vec4(position, 1.0);
    vertColor = color;
    vertNormal = normalize(push.transform * vec4(normal, 0.0)).xyz;
}