#ifndef __PIPELINE
#define __PIPELINE

#include "Helpers.h"
#include "Device.h"
#include "Model.h"

// input assembler -> vertex shader -> rasterization -> fragment shader -> color blending
namespace Eng {
    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo& copy) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo& copy) = delete;
        PipelineConfigInfo(PipelineConfigInfo&& move) = delete;
        PipelineConfigInfo& operator=(PipelineConfigInfo&& move) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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

        static void createDefaultConfig(PipelineConfigInfo& config);
    };
}

#endif