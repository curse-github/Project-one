#version 450
#extension GL_EXT_nonuniform_qualifier : require

struct Light {
    vec4 position;
    vec4 colorIntensity;
};
struct Material {
    vec4 diffuseColor_Transparency;
    vec4 specColor_Exp;
    uint map_diff;
    uint map_specC;
    uint map_specE;
    uint map_norm;
};
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
layout (constant_id = 0) const int MAX_LIGHTS = 1;
layout (constant_id = 1) const int NUM_TEXTURES = 1;
layout (constant_id = 2) const int NUM_MATERIALS = 1;


layout (location = 0) in vec2 fragUv;
layout (location = 1) in vec3 fragTangent;
layout (location = 2) in vec3 fragNormal;
layout (location = 3) in vec3 fragWorldPosition;

layout(set = 0, binding = 0) uniform GlobalUboData {
    mat4 projectionView;
    mat4 inverseView;
    vec4 ambientLightColor;
    uint numLights;
    Light lights[MAX_LIGHTS];
};

layout(set = 1, binding = 0) uniform sampler2D texSamplers[NUM_TEXTURES];
layout(set = 1, binding = 1) uniform Materials { Material materials[NUM_MATERIALS]; };
layout(set = 2, binding = 0) uniform Idxs { uint matIdx; };

layout (location = 0) out vec4 outColor;
void main() {
    // maybe something to loop at
    // https://docs.vulkan.org/samples/latest/samples/extensions/descriptor_indexing/README.html
    Material mat = materials[nonuniformEXT(matIdx)];
    vec4 diffuseColor = mat.diffuseColor_Transparency*texture(texSamplers[nonuniformEXT(mat.map_diff)], fragUv);
    // initialize some stuff for lights
    vec3 ogNormal = normalize(fragNormal);
    vec3 diffuseLightColor = ambientLightColor.xyz*ambientLightColor.w;
    vec3 specularLightColor = vec3(0.0);
    vec3 viewDirection = normalize(inverseView[3].xyz-fragWorldPosition);
    vec4 specColor = mat.specColor_Exp*mat.diffuseColor_Transparency*texture(texSamplers[nonuniformEXT(mat.map_specC)], fragUv);
    float N = (mat.specColor_Exp.w+0.001)*texture(texSamplers[nonuniformEXT(mat.map_specE)], fragUv).r;
    float S = (N+8)/25.1327412287;
    // loop through lights
    for(uint i = 0; i < numLights; i++) {
        // lighting data
        Light light = lights[i];
        vec3 lightDirection = light.position.xyz - fragWorldPosition;
        float attenuation = max(dot(lightDirection, lightDirection), 0.25);
        lightDirection = normalize(lightDirection);
        vec3 lightColor = light.colorIntensity.xyz*light.colorIntensity.w * max(dot(ogNormal, lightDirection), 0.0) / attenuation;
        // diffuse light
        diffuseLightColor += lightColor;
        // specular light
        float blinnPhongTerm = clamp(dot(ogNormal, normalize(lightDirection + viewDirection)), 0, 1);// normal . halfAngle
        specularLightColor += lightColor*pow(blinnPhongTerm, N)*S;
    }
    outColor = vec4(toneMap(diffuseLightColor*diffuseColor.rgb + specularLightColor*specColor.rgb), diffuseColor.a);
}