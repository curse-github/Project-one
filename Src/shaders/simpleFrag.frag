#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragNormal;

layout (location = 0) out vec4 outColor;

void main() {
    float lightAmount = max(dot(fragNormal, vec3(0.0, 0.0, -1.0)), 0.0);
    lightAmount = lightAmount*0.875+0.125;
    vec3 color = fragColor*lightAmount;
    outColor = vec4(color, 1.0);
}