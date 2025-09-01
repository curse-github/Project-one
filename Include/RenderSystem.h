#ifndef __RENDERSYSTEM
#define __RENDERSYSTEM

#include "Helpers.h"
#include "Device.h"
#include "Pipeline.h"
#include "Model.h"
#include "GameObject.h"
#include "Camera.h"

namespace Eng {
    struct SimplePushConstantData {
        glm::mat4 projection{1.0f};
        glm::mat4 transform{1.0f};
    };
    class SimpleRenderSystem {
        Device* device;
        Pipeline* pipeline;
        VkPipelineLayout pipelineLayout;

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
        void recordCommandBuffer(const int& imageIndex);
    public:
        SimpleRenderSystem(Device* _device, VkRenderPass renderPass);
        SimpleRenderSystem(const SimpleRenderSystem& copy) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem& copy) = delete;
        SimpleRenderSystem(SimpleRenderSystem&& move) = delete;
        SimpleRenderSystem& operator=(SimpleRenderSystem&& move) = delete;
        ~SimpleRenderSystem();
        
        void recordObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& objects, const Camera& camera);
    };
}

#endif// __RENDERSYSTEM