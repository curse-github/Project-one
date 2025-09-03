#include "RenderSystem.h"

namespace Eng {
    SimpleRenderSystem::SimpleRenderSystem(Device* _device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout)
        : device(_device), pipeline(nullptr)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalDescriptorSetLayout};

        // create layout
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        // used for sending any data to GPU, besides vertex data.
        pipelineLayoutInfo.setLayoutCount = static_cast<unsigned int>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        // used for sending small amounts of data.
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device->device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("failed to create pipeline layout!");
        // create pipeline
        assert((pipelineLayout != VK_NULL_HANDLE) && "cannot create pipeline before pipeline layout.");
        if (pipeline != nullptr) delete pipeline;
        PipelineConfigInfo pipelineConfig{};
        Pipeline::createDefaultConfig(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = new Pipeline(device, "shaders/simpleVert.vert.spv", "shaders/simpleFrag.frag.spv", pipelineConfig);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {
        delete pipeline;
        vkDestroyPipelineLayout(device->device, pipelineLayout, nullptr);
    }
    void SimpleRenderSystem::recordObjects(FrameInfo& frameInfo) {
        pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        for (std::pair<const GameObject::id_t, GameObject>& kv : *frameInfo.objects) {
            GameObject& object = kv.second;
            SimplePushConstantData push{object.transform.getTransformMat(), object.transform.getNormalMat()};
            vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            object.mesh->bind(frameInfo.commandBuffer);
            object.mesh->draw(frameInfo.commandBuffer);
        }
    }
}