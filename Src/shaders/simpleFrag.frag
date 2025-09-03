#version 450

layout (location = 0) in vec3 fragNormal;
layout (location = 1) in vec3 lightDirection;

layout (location = 0) out vec4 outColor;

const float AMBIENT = 0.125;

void main() {
    float lightIntensity = max(dot(normalize(fragNormal), lightDirection), 0.0);
    lightIntensity = lightIntensity*(1.0-AMBIENT)+AMBIENT;

    outColor = vec4(lightIntensity*vec3(0.8, 0.8, 0.8), 1.0);
}