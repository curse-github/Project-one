#ifndef ENG_DESCRIPTORS
#define ENG_DESCRIPTORS

#include "Helpers.h"
#include "Device.h"

namespace Eng {
    class DescriptorSetLayout {
        Device* device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
        friend class DescriptorWriter;
    public:
        class Builder {
            Device* device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        public:
            Builder(Device* _device);
            Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
            DescriptorSetLayout* build() const;
        };
        DescriptorSetLayout(Device* _device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~DescriptorSetLayout();

        DescriptorSetLayout(const DescriptorSetLayout& copy) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout& copy) = delete;
        DescriptorSetLayout(DescriptorSetLayout&& move) = delete;
        DescriptorSetLayout& operator=(DescriptorSetLayout&& move) = delete;
        
        VkDescriptorSetLayout descriptorSetLayout;
    };
    
    class DescriptorPool {
        Device* device;
        VkDescriptorPool descriptorPool;
        friend class DescriptorWriter;
        public:
        class Builder {
            Device* device;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        public:
            Builder(Device* _device);
            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            DescriptorPool* build() const;
        };
        DescriptorPool(Device* _device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~DescriptorPool();
        DescriptorPool(const DescriptorPool& copy) = delete;
        DescriptorPool& operator=(const DescriptorPool& copy) = delete;
        DescriptorPool(DescriptorPool&& move) = delete;
        DescriptorPool& operator=(DescriptorPool&& move) = delete;

        bool allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
        void freeDescriptorSet(std::vector<VkDescriptorSet>& descriptors) const;
        void resetPool();
    };

    class DescriptorWriter {
        DescriptorSetLayout* setLayout;
        DescriptorPool* pool;
        std::vector<VkWriteDescriptorSet> writes;
        public:
        DescriptorWriter(DescriptorSetLayout* _setLayout, DescriptorPool* _pool);

        DescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        DescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);
    };
}

#endif// ENG_DESCRIPTORS