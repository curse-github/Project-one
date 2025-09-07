#version 450
#extension GL_EXT_nonuniform_qualifier : require

vec3 toneMap(vec3 x) {
    // ACES tone mapping
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

layout (location = 0) in vec2 fragUv;
layout (location = 1) in vec3 fragNormal;
layout (location = 2) in vec3 fragWorldPosition;

layout (location = 0) out vec4 outColor;

layout (constant_id = 0) const int MAX_LIGHTS = 1;
layout (constant_id = 1) const int NUM_TEXTURES = 1;

layout(set = 1, binding = 0) uniform sampler2D texSamplers[NUM_TEXTURES];
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
layout(set = 2, binding = 0) uniform MaterialInfo {
    vec4 spec;
    uint texIdx;
};

void main() {
    vec3 normalNormal = normalize(fragNormal);
    vec3 diffuseLightColor = ambientLightColor.xyz*ambientLightColor.w;
    vec3 specularLightColor = vec3(0.0);
    vec3 viewDirection = normalize(inverseView[3].xyz-fragWorldPosition);
    float N = spec.w;
    float S = (N+8)/25.1327412287;
    for(uint i = 0; i < numLights; i++) {
        Light light = lights[i];
        vec3 lightDirection = light.position.xyz - fragWorldPosition;
        float attenuation = max(dot(lightDirection, lightDirection), 0.25);
        lightDirection = normalize(lightDirection);
        vec3 lightColor = light.colorIntensity.xyz*light.colorIntensity.w * max(dot(normalNormal, lightDirection), 0.0) / attenuation;

        diffuseLightColor += lightColor;

        float blinnPhongTerm = clamp(dot(normalNormal, normalize(lightDirection + viewDirection)), 0, 1);// normal . halfAngle
        specularLightColor += lightColor*pow(blinnPhongTerm, N)*S;
    }
    vec4 color = texture(texSamplers[nonuniformEXT(texIdx)], fragUv);
    outColor = vec4(toneMap(diffuseLightColor*color.rgb + specularLightColor*spec.rgb), color.a);
}