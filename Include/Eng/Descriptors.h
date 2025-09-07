#ifndef ENG_DESCRIPTORS
#define ENG_DESCRIPTORS

#include "Helpers.h"
#include "Device.h"

namespace Eng {
    class DescriptorSetLayout {
        Device* device;
        std::unordered_map<unsigned int, VkDescriptorSetLayoutBinding> bindings;
        friend class DescriptorWriter;
    public:
        class Builder {
            Device* device;
            std::unordered_map<unsigned int, VkDescriptorSetLayoutBinding> bindings{};
        public:
            Builder(Device* _device);
            Builder& addBinding(const unsigned int& binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, const unsigned int& count = 1);
            DescriptorSetLayout* build() const;
        };
        DescriptorSetLayout(Device* _device, std::unordered_map<unsigned int, VkDescriptorSetLayoutBinding> bindings);
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
            unsigned int maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        public:
            Builder(Device* _device);
            Builder& addPoolSize(VkDescriptorType descriptorType, const unsigned int& count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(const unsigned int& count);
            DescriptorPool* build() const;
        };
        DescriptorPool(Device* _device, const unsigned int& maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
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

        DescriptorWriter& writeBuffer(const unsigned int& binding, VkDescriptorBufferInfo *bufferInfo);
        DescriptorWriter& writeImage(const unsigned int& binding, VkDescriptorImageInfo *imageInfo);
        DescriptorWriter& writeImages(const unsigned int& binding, VkDescriptorImageInfo *imageInfo, const size_t& count);
        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);
    };
}

#endif// ENG_DESCRIPTORS