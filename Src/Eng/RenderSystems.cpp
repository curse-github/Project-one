#include "RenderSystems.h"

namespace Eng {
    DiffuseBlinnPhongRenderSystem::DiffuseBlinnPhongRenderSystem(Device* _device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout)
        : device(_device), pipeline(nullptr)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(DefaultPushConstantData);

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
        Pipeline::configSetDefaults(pipelineConfig);
        pipelineConfig.bindingDescriptions = Mesh::Vertex::getBindingDescriptions();
        pipelineConfig.attributeDescriptions = Mesh::Vertex::getAttributeDescriptions();
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = new Pipeline(device, "shaders/Diffuse-Blinn-Phong.vert.spv", "shaders/Diffuse-Blinn-Phong.frag.spv", pipelineConfig);
    }

    DiffuseBlinnPhongRenderSystem::~DiffuseBlinnPhongRenderSystem() {
        delete pipeline;
        vkDestroyPipelineLayout(device->device, pipelineLayout, nullptr);
    }
    void DiffuseBlinnPhongRenderSystem::recordObjects(FrameInfo& frameInfo) {
        pipeline->bind(frameInfo.commandBuffer);
        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);
        for (std::pair<const GameObject::id_t, GameObject>& kv : *frameInfo.objects) {
            GameObject& object = kv.second;
            DefaultPushConstantData push{object.transform.getTransformMat(), object.transform.getNormalMat()};
            vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(DefaultPushConstantData), &push);
            object.mesh->bind(frameInfo.commandBuffer);
            object.mesh->draw(frameInfo.commandBuffer);
        }
    }

    
    PointLightRenderSystem::PointLightRenderSystem(Device* _device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout)
        : device(_device), pipeline(nullptr)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PointLightPushConstantData);

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
        Pipeline::configSetDefaults(pipelineConfig);
        Pipeline::configEnableAlphaBlending(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = new Pipeline(device, "shaders/PointLight.vert.spv", "shaders/PointLight.frag.spv", pipelineConfig);
    }

    PointLightRenderSystem::~PointLightRenderSystem() {
        delete pipeline;
        vkDestroyPipelineLayout(device->device, pipelineLayout, nullptr);
    }
#define sortedGameObjectIdsT std::map<float, GameObject::id_t>
    sortedGameObjectIdsT sortedIds{};
    void PointLightRenderSystem::recordObjects(FrameInfo& frameInfo) {
        pipeline->bind(frameInfo.commandBuffer);
        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);
        vec3 cameraPos = frameInfo.camera->getPosition();
        sortedIds.clear();
        for (std::pair<const GameObject::id_t, GameObject>& kv : *frameInfo.lights) {
            GameObject& light = kv.second;
            vec3 distance = cameraPos - light.transform.position;
            sortedIds[dot(distance,distance)] = light.id;
        }
        for (std::reverse_iterator<sortedGameObjectIdsT::iterator> it = sortedIds.rbegin(); it != sortedIds.rend(); ++it) {
            GameObject& light = frameInfo.lights->at(it->second);
            PointLightPushConstantData push{vec4(light.transform.position, light.transform.scale.x), light.light->colorIntensity};
            vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstantData), &push);
            vkCmdDraw(frameInfo.commandBuffer, 3, 1, 0, 0);
        }
    }
}