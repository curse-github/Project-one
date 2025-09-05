#ifndef ENG_RENDERSYSTEMS
#define ENG_RENDERSYSTEMS

#include "Helpers.h"
#include "Device.h"
#include "Pipeline.h"
#include "Mesh.h"
#include "GameObject.h"
#include "FrameInfo.h"

namespace Eng {
    struct DefaultPushConstantData {
        glm::mat4 modelMat{1.0f};
        glm::mat4 normalMat{1.0f};
    };
    class DiffuseBlinnPhongRenderSystem {
        Device* device;
        Pipeline* pipeline;
        VkPipelineLayout pipelineLayout;

        void recordCommandBuffer(const int& imageIndex);
    public:
        DiffuseBlinnPhongRenderSystem(Device* _device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout);
        DiffuseBlinnPhongRenderSystem(const DiffuseBlinnPhongRenderSystem& copy) = delete;
        DiffuseBlinnPhongRenderSystem& operator=(const DiffuseBlinnPhongRenderSystem& copy) = delete;
        DiffuseBlinnPhongRenderSystem(DiffuseBlinnPhongRenderSystem&& move) = delete;
        DiffuseBlinnPhongRenderSystem& operator=(DiffuseBlinnPhongRenderSystem&& move) = delete;
        ~DiffuseBlinnPhongRenderSystem();
        
        void recordObjects(FrameInfo& frameInfo);
    };
    struct PointLightPushConstantData {
        vec4 positionSize{0.0f};
        vec4 colorIntensity{0.0f};
    };
    class PointLightRenderSystem {
        Device* device;
        Pipeline* pipeline;
        VkPipelineLayout pipelineLayout;

        void recordCommandBuffer(const int& imageIndex);
    public:
        PointLightRenderSystem(Device* _device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout);
        PointLightRenderSystem(const PointLightRenderSystem& copy) = delete;
        PointLightRenderSystem& operator=(const PointLightRenderSystem& copy) = delete;
        PointLightRenderSystem(PointLightRenderSystem&& move) = delete;
        PointLightRenderSystem& operator=(PointLightRenderSystem&& move) = delete;
        ~PointLightRenderSystem();
        
        void recordObjects(FrameInfo& frameInfo);
    };
}

#endif// ENG_RENDERSYSTEMS