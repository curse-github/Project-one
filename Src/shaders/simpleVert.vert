#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec3 vertNormal;
layout (location = 1) out vec3 vertPosition;

layout(push_constant) uniform Push {
    mat4 modelMat;
    mat4 normalMat;
} push;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
    mat4 projectionView;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
} Gubo;

void main() {
    vec4 worldPosition = push.modelMat * vec4(position, 1.0);
    gl_Position = Gubo.projectionView * worldPosition;
    vertNormal = mat3(push.normalMat) * normal;
    vertPosition = worldPosition.xyz;
}