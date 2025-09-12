#ifndef __FRAMEINFO
#define __FRAMEINFO

#include "Helpers.h"
#include "GameObject.h"

namespace Eng {
    struct FrameInfo {
        unsigned int frameIndex;
        float t;
        float dt;
        VkCommandBuffer commandBuffer;
        Camera* camera;
        VkDescriptorSet globalDescriptorSet;
        VkDescriptorSet materialDescriptorSet;
        GameObject::Map* objects;
        GameObject::Map* lights;
    };
}

#endif// __FRAMEINFO