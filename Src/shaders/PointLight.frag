#version 450

layout (location = 0) in vec3 fragPosition;

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    vec4 positionScale;
    vec4 colorIntensity;
} push;

layout (constant_id = 0) const int MAX_LIGHTS = 1;
struct Light {
    vec4 position;
    vec4 colorIntensity;
};
layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
    mat4 projectionView;
    mat4 inverseView;
    vec4 ambientLightColor;
    uint numLights;
    Light lights[MAX_LIGHTS];
} Gubo;

const float PI = 3.14159265359;
void main() {
    float dist = length(fragPosition);
    if (dist > 1) discard;
    float cosDist = 0.5*(cos(dist*PI)+1);
    outColor = vec4(push.colorIntensity.xyz+cosDist, cosDist);
}