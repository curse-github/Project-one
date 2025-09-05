#include "Renderer.h"
namespace Eng {
    Renderer::Renderer(Window* _window, Device* _device)
        : window(_window), device(_device), swapchain(nullptr)
    {
        recreateSwapchain();
        createCommandBuffers();
    }

    Renderer::~Renderer() {
        delete swapchain;
    }
    void Renderer::recreateSwapchain() {
        // IMPORTANT: this halts the program while minimized.
        while ((window->size.x == 0) || (window->size.y == 0))
            glfwWaitEvents();
        vkDeviceWaitIdle(device->device);
        // recreate the swapchain
        if (swapchain == nullptr) swapchain = new Swapchain(device, VkExtent2D{static_cast<unsigned int>(window->size.x), static_cast<unsigned int>(window->size.y)});
        else {
            Swapchain* oldSwapchain = swapchain;
            swapchain = new Swapchain(device, VkExtent2D{static_cast<unsigned int>(window->size.x), static_cast<unsigned int>(window->size.y)}, oldSwapchain);
            delete oldSwapchain;
            if (!oldSwapchain->swapchainsCompatible(*swapchain))
                // should at some point just recreate the pipeline/rendersystems
                throw std::runtime_error("swapchain image format has changed!");
        }
        // recreatePipeline();
    }
    void Renderer::freeCommandBuffers() {
        vkFreeCommandBuffers(device->device, device->commandPool, static_cast<unsigned int>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }
    void Renderer::createCommandBuffers() {
        commandBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        // primary buffers can be submitted for execution but cannot be called by other buffers
        // secondary buffers cannot be submitted for execution directly but can be called by other buffers
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device->commandPool;
        allocInfo.commandBufferCount = static_cast<unsigned int>(commandBuffers.size());
        if (vkAllocateCommandBuffers(device->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate command buffers!");
    }
    VkCommandBuffer Renderer::beginFrame() {
        assert(!frameInProgress && "Cant begin frame when it is has already started.");
        VkResult result = swapchain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapchain(); return VK_NULL_HANDLE;
        }
        frameInProgress = true;
        if ((result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR))
            throw std::runtime_error("failed to aquire next swapchain image!");
        VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        swapchain->waitForCommandBuffer();
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("failed to begin recording command buffer!");
        return commandBuffer;
    }
    void Renderer::beginRenderPass(VkCommandBuffer commandBuffer) {
        assert(frameInProgress && "Cant begin render pass when frame has not started.");
        assert(!renderPassInProgress && "Cant begin render pass when it has already started.");
        assert((commandBuffer == getCurrentCommandBuffer()) && "Cant begin render pass on command buffer for a different frame.");
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapchain->renderPass;
        renderPassInfo.framebuffer = swapchain->swapChainFramebuffers[currentImageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {swapchain->swapChainExtent.width, swapchain->swapChainExtent.height};
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = clearColor;
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<unsigned int>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        // viewport, IMPORTANT, very useful for moving the viewport around and squishing it
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapchain->swapChainExtent.width);
        viewport.height = static_cast<float>(swapchain->swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        // scissor
        VkRect2D scissor{{0, 0}, swapchain->swapChainExtent};
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        renderPassInProgress = true;
    }
    void Renderer::endRenderPass(VkCommandBuffer commandBuffer) {
        assert(renderPassInProgress && "Cant end render pass when it has not started.");
        assert(frameInProgress && "Cant end render pass when frame has not started.");
        assert((commandBuffer == getCurrentCommandBuffer()) && "Cant end render pass on command buffer for a different frame.");
        vkCmdEndRenderPass(commandBuffer);
        renderPassInProgress = false;
    }
    void Renderer::endFrame() {
        assert(frameInProgress && "Cant end frame when it is has not started.");
        VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to record command buffer!");
        VkResult result = swapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR) || (window->frameBufferResized)) {
            window->frameBufferResized = false;
            recreateSwapchain();
        } else if (result != VK_SUCCESS)
            throw std::runtime_error("failed to present next swapchain image!");
        frameInProgress = false;
    }
};