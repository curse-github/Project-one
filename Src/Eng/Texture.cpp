#include "Texture.h"

namespace Eng {
    Texture::Texture(Device* _device, const unsigned int& _width, const unsigned int& _height, const void* data) : device(_device), width(_width), height(_height) {
        VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
        VkImageAspectFlags aspactFlags = VK_IMAGE_ASPECT_COLOR_BIT;
        VkFilter minFilter = VK_FILTER_LINEAR;
        VkFilter maxFilter = VK_FILTER_LINEAR;
        VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        unsigned int size = width*height;
        Buffer stagingBuffer(device, 4, width*height, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0);
        stagingBuffer.map();
        stagingBuffer.write(data, width*height);
        stagingBuffer.unmap();
        device->createImage(width, height, format, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, GPUmemory);
        device->transitionImageLayout(image, aspactFlags, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        device->copyBufferToImage(stagingBuffer.getBuffer(), image, aspactFlags, width, height);
        device->transitionImageLayout(image, aspactFlags, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = aspactFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(device->device, &viewInfo, nullptr, &view) != VK_SUCCESS)
            throw std::runtime_error("failed to create image view!");

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = maxFilter;
        samplerInfo.minFilter = minFilter;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.addressModeU = addressMode;
        samplerInfo.addressModeV = addressMode;
        samplerInfo.addressModeW = addressMode;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1u;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;// optional, since campare is disabled
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        if (vkCreateSampler(device->device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
            throw std::runtime_error("failed to create sampler!");
    }
    Texture::~Texture() {
        vkDestroySampler(device->device, sampler, nullptr);
        vkDestroyImageView(device->device, view, nullptr);
        vkDestroyImage(device->device, image, nullptr);
        vkFreeMemory(device->device, GPUmemory, nullptr);
    }
    
    VkDescriptorImageInfo Texture::descriptorInfo() {
        return VkDescriptorImageInfo{
            sampler,
            view,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };
    }
}