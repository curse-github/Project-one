#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec2 vertUv;
layout (location = 1) out vec3 vertNormal;
layout (location = 2) out vec3 vertWorldPosition;

layout(push_constant) uniform PushVert {
    mat4 modelMat;
    mat4 normalMat;
} pushvert;

layout (constant_id = 0) const uint MAX_LIGHTS = 1;
struct Light {
    vec4 position;
    vec4 colorIntensity;
};
layout(set = 0, binding = 0) uniform GlobalUboData {
    mat4 projectionView;
    mat4 inverseView;
    vec4 ambientLightColor;
    uint numLights;
    Light lights[MAX_LIGHTS];
};

void main() {
    vec4 worldPosition = pushvert.modelMat * vec4(position, 1.0);
    gl_Position = projectionView * worldPosition;
    vertUv = uv;
    vertNormal = mat3(pushvert.normalMat) * normal;
    vertWorldPosition = worldPosition.xyz/worldPosition.w;// perspective divide
}