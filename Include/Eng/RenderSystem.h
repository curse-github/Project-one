#ifndef ENG_RENDERSYSTEM
#define ENG_RENDERSYSTEM

#include "Helpers.h"
#include "Device.h"
#include "Pipeline.h"
#include "Mesh.h"
#include "GameObject.h"

namespace Eng {
    struct SimplePushConstantData {
        glm::mat4 modelMat{1.0f};
        glm::mat4 normalMat{1.0f};
    };
    class SimpleRenderSystem {
        Device* device;
        Pipeline* pipeline;
        VkPipelineLayout pipelineLayout;

        void recordCommandBuffer(const int& imageIndex);
    public:
        SimpleRenderSystem(Device* _device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout);
        SimpleRenderSystem(const SimpleRenderSystem& copy) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem& copy) = delete;
        SimpleRenderSystem(SimpleRenderSystem&& move) = delete;
        SimpleRenderSystem& operator=(SimpleRenderSystem&& move) = delete;
        ~SimpleRenderSystem();
        
        void recordObjects(FrameInfo& frameInfo, std::vector<GameObject>& objects);
    };
}

#endif// ENG_RENDERSYSTEM