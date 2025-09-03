#ifndef ENG_RENDERER
#define ENG_RENDERER

#include "Helpers.h"
#include "Window.h"
#include "Device.h"
#include "Swapchain.h"
#include "Mesh.h"

namespace Eng {
    class Renderer {
        Window* window;
        Device* device;
        Swapchain* swapchain;
        std::vector<VkCommandBuffer> commandBuffers;
        unsigned int currentImageIndex;

        void recreateSwapchain();
        void freeCommandBuffers();
        void createCommandBuffers();
    public:
        Renderer(Window* _window, Device* _device);
        Renderer(const Renderer& copy) = delete;
        Renderer& operator=(const Renderer& copy) = delete;
        Renderer(Renderer&& move) = delete;
        Renderer& operator=(Renderer&& move) = delete;
        ~Renderer();

        VkClearColorValue clearColor{0.0f, 0.0f, 0.0f, 1.0f};
        bool frameInProgress = false;
        bool renderPassInProgress = false;
        unsigned int getFrame() {
            assert(frameInProgress && "cannot not get frame when frame has not started");
            return swapchain->currentFrame;
        };
        VkCommandBuffer getCurrentCommandBuffer() {
            assert(frameInProgress && "cannot not get command buffer when frame has not started");
            return commandBuffers[swapchain->currentFrame];
        }
        VkRenderPass getRenderPass() {
            return swapchain->renderPass;
        }
        float getAspectRatio() { return (float)swapchain->swapChainExtent.width/swapchain->swapChainExtent.height; };

        VkCommandBuffer beginFrame();
        void beginRenderPass(VkCommandBuffer commandBuffer);
        void endRenderPass(VkCommandBuffer commandBuffer);
        void endFrame();
    };
}

#endif// ENG_RENDERER