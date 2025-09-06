#version 450

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
layout(set = 0, binding = 1) uniform sampler2D texSampler;

vec3 specularColor = vec3(1.0, 1.0, 1.0);

void main() {
    vec3 normalNormal = normalize(fragNormal);
    vec3 diffuseLightColor = Gubo.ambientLightColor.xyz*Gubo.ambientLightColor.w;
    vec3 specularLightColor = vec3(0.0);
    vec3 viewDirection = normalize(Gubo.inverseView[3].xyz-fragWorldPosition);
    for(uint i = 0; i < Gubo.numLights; i++) {
        Light light = Gubo.lights[i];
        vec3 lightDirection = light.position.xyz - fragWorldPosition;
        float attenuation = max(dot(lightDirection, lightDirection), 0.25);
        lightDirection = normalize(lightDirection);
        vec3 lightColor = light.colorIntensity.xyz*light.colorIntensity.w * max(dot(normalNormal, lightDirection), 0.0) / attenuation;

        diffuseLightColor += lightColor;

        float blinnPhongTerm = dot(normalNormal, normalize(lightDirection + viewDirection));// normal . halfAngle
        float N = 400.0;
        float S = (N/4.0+8)/25.1327412287;
        specularLightColor += lightColor*pow(clamp(blinnPhongTerm, 0, 1), N)*S;
    }
    vec4 color = texture(texSampler, fragUv);
    outColor = vec4(toneMap(diffuseLightColor*color.rgb + specularLightColor*specularColor), color.a);
}