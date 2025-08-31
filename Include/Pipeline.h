#ifndef __PIPELINE
#define __PIPELINE

#include "Helpers.h"
#include "Device.h"
#include "Model.h"

// input assembler -> vertex shader -> rasterization -> fragment shader -> color blending
namespace Eng {
    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkRenderPass renderPass = VK_NULL_HANDLE;
        unsigned int subpass = 0;
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
        void bind(VkCommandBuffer commandBuffer);

        static PipelineConfigInfo createDefaultConfig(const ivec2& windowSize);
    };
}

#endif