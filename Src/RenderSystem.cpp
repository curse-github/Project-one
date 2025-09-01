#include "RenderSystem.h"

namespace Eng {
    SimpleRenderSystem::SimpleRenderSystem(Device* _device, VkRenderPass renderPass)
        : device(_device), pipeline(nullptr)
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {
        delete pipeline;
        vkDestroyPipelineLayout(device->device, pipelineLayout, nullptr);
    }
    
    void SimpleRenderSystem::createPipelineLayout() {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        // used for sending any data to GPU, besides vertex data.
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        // used for sending small amounts of data.
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device->device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("failed to create pipeline layout!");
    }
    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert((pipelineLayout != VK_NULL_HANDLE) && "cannot create pipeline before pipeline layout.");
        if (pipeline != nullptr) delete pipeline;
        PipelineConfigInfo pipelineConfig{};
        Pipeline::createDefaultConfig(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = new Pipeline(device, "shaders/simpleVert.vert.spv", "shaders/simpleFrag.frag.spv", pipelineConfig);
    }
    void SimpleRenderSystem::recordObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& objects, const Camera& camera) {
        pipeline->bind(commandBuffer);
        mat4 projectionView = camera.projection*camera.view;
        for (GameObject& object : objects) {
            SimplePushConstantData push{projectionView, object.transform.getMat()};
            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            object.model->bind(commandBuffer);
            object.model->draw(commandBuffer);
        }
    }
}