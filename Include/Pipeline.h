#ifndef __PIPELINE
#define __PIPELINE

#include <cassert>

#include "Helpers.h"
#include "Device.h"

namespace Eng {
    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkRenderPass renderPass = VK_NULL_HANDLE;
        uint32_t subpass = 0;
    };
    class Pipeline {
        Device* device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
        public:
        Pipeline(Device* _device, const std::string& vert, const std::string& frag, const PipelineConfigInfo& config);
        Pipeline(const Pipeline& copy) = delete;
        Pipeline& operator=(const Pipeline& copy) = delete;
        Pipeline(Pipeline&& move) = delete;
        Pipeline& operator=(Pipeline&& move) = delete;
        ~Pipeline();
        
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
        static PipelineConfigInfo createDefaultConfig(const ivec2& windowSize);
    };
}

#endif