#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 color;

layout (location = 0) out vec3 vertColor;
layout (location = 1) out vec3 vertNormal;

layout(push_constant) uniform Push {
    mat4 transform;
    mat4 normalMat;
} push;

void main() {
    gl_Position = push.transform * vec4(position, 1.0);
    
    vertNormal = mat3(push.normalMat) * normal;

    vertColor = color;
}