#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragNormal;

layout (location = 0) out vec4 outColor;

const float AMBIENT = 0.125;
const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));

void main() {
    float lightIntensity = max(dot(normalize(fragNormal), vec3(0.0, 0.0, -1.0)), 0.0);
    lightIntensity = lightIntensity*(1.0-AMBIENT)+AMBIENT;

    outColor = vec4(lightIntensity*fragColor, 1.0);
}