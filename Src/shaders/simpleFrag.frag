#version 450

layout (location = 0) in vec3 fragNormal;
layout (location = 1) in vec3 fragPosition;

layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
    mat4 projectionView;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
} Gubo;

void main() {
    vec3 diffuseLightDirection = Gubo.lightPosition - fragPosition;
    float lightIntensity = max(dot(normalize(fragNormal), normalize(diffuseLightDirection)), 0.0) / dot(diffuseLightDirection, diffuseLightDirection);
    vec3 ambientLight = Gubo.ambientLightColor.xyz*Gubo.ambientLightColor.w;
    vec3 diffuseLight = Gubo.lightColor.xyz*Gubo.lightColor.w * lightIntensity;
    outColor = vec4((ambientLight + diffuseLight)*vec3(0.8, 0.8, 0.8), 1.0);
}