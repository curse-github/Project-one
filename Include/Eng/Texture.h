#ifndef __TEXTURE
#define __TEXTURE

#include "Helpers.h"
#include "Device.h"
#include "Buffer.h"

namespace Eng {
    class Texture {
        Device* device;
        VkSampler sampler = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory GPUmemory = VK_NULL_HANDLE;
    public:
        Texture(Device* _device, const unsigned int& _width, const unsigned int& _height, const void* data);
        Texture(const Texture& copy) = delete;
        Texture& operator=(const Texture& copy) = delete;
        Texture(Texture&& move) = default;
        Texture& operator=(Texture&& move) = default;
        ~Texture();

        unsigned int width;
        unsigned int height;
        
        VkDescriptorImageInfo descriptorInfo();
    };
}

#endif