#ifndef __FRAMEINFO
#define __FRAMEINFO

#include "Helpers.h"
#include "GameObject.h"

namespace Eng {
    struct FrameInfo {
        unsigned int frameIndex;
        float dt;
        VkCommandBuffer commandBuffer;
        Camera* camera;
        VkDescriptorSet globalDescriptorSet;
        GameObject::Map* objects;
    };
}

#endif// __FRAMEINFO