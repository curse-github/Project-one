#ifndef __FRAMEINFO
#define __FRAMEINFO

#include "Helpers.h"
#include "GameObject.h"

namespace Eng {
    #define MAX_LIGHTS 10u
    struct pointLight {
        vec4 positionSize{0.0f};
        vec4 colorIntensity{0.0f};
    };
    struct GlobalUniformBufferObject {
        mat4 projectionView{1.0f};
        mat4 inverseView{1.0f};
        vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.2f};
        unsigned int numLights{0};
        alignas(16) pointLight pointLights[MAX_LIGHTS];
    };
    struct FrameInfo {
        unsigned int frameIndex;
        float t;
        float dt;
        VkCommandBuffer commandBuffer;
        Camera* camera;
        VkDescriptorSet globalUboDescriptorSet;
        GameObject::Map* objects;
        GameObject::Map* lights;
    };
}

#endif// __FRAMEINFO