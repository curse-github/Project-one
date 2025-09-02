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

const float AMBIENT = 0.125;
const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));

void main() {
    gl_Position = push.transform * vec4(position, 1.0);
    
    vertNormal = normalize(mat3(push.normalMat) * normal);
    float lightIntensity = max(dot(vertNormal, vec3(0.0, 0.0, -1.0)), 0.0);
    lightIntensity = lightIntensity*(1.0-AMBIENT)+AMBIENT;

    vertColor = lightIntensity*color;
}