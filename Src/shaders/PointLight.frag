#version 450

layout (location = 0) in vec3 fragPosition;

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    vec4 positionScale;
    vec4 colorIntensity;
};

const float PI = 3.14159265359;
void main() {
    float dist = length(fragPosition);
    if (dist > 1) discard;
    float cosDist = 0.5*(cos(dist*PI)+1);
    outColor = vec4(colorIntensity.xyz+cosDist, cosDist);
}