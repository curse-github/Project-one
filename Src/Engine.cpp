#include "Engine.h"
#include <iostream>

namespace Eng {
    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
        : window(windowName, windowSize), device(&window), swapchain(nullptr), pipeline(nullptr)
    {
    }

    Engine::~Engine() {
        for (Model*& model : models)
            delete model;
        delete pipeline;
        delete swapchain;
        vkDestroyPipelineLayout(device.device, pipelineLayout, nullptr);
        glfwTerminate();
    }
    void Engine::start() {
        createPipelineLayout();
        recreateSwapchain();
        createCommandBuffers();
        started = true;
    }
    void Engine::loop() {
        while(!window.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(device.device);
    }



    
    void Engine::addModel(const std::vector<Model::Vertex>& vertices) {
        if (started) return;
        models.push_back(new Model(&device, vertices));
    }
    void Engine::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        // used for sending any data to GPU, besides vertex data.
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        // used for sending small amounts of data.
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(device.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("failed to create pipeline layout!");
    }
    void Engine::recreateSwapchain() {
        // IMPORTANT: this halts the program while minimized.
        while ((window.size.x == 0) || (window.size.y == 0))
            glfwWaitEvents();
        vkDeviceWaitIdle(device.device);
        // recreate the swapchain
        if (swapchain == nullptr) swapchain = new Swapchain(&device, VkExtent2D{static_cast<unsigned int>(window.size.x), static_cast<unsigned int>(window.size.y)});
        else {
            Swapchain* temp = swapchain;
            swapchain = new Swapchain(&device, VkExtent2D{static_cast<unsigned int>(window.size.x), static_cast<unsigned int>(window.size.y)}, temp);
            delete temp;
            if (swapchain->swapChainImages.size() != commandBuffers.size()) {
                freeCommandBuffers();
                createCommandBuffers();
            }
#ifdef _DEBUG
            else {
                // std::cout << "created new swapchain without creating new render pass\n";
            }
#endif
        }
        recreatePipeline();
    }
    void Engine::recreatePipeline() {
        assert((swapchain != nullptr) && "cannot create pipeline before swap chain.");
        assert((pipelineLayout != VK_NULL_HANDLE) && "cannot create pipeline before swap chain.");
        if (pipeline != nullptr) delete pipeline;
        PipelineConfigInfo pipelineConfig{};
        Pipeline::createDefaultConfig(pipelineConfig);
        pipelineConfig.renderPass = swapchain->renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = new Pipeline(&device, "out/shaders/simpleVert.vert.spv", "out/shaders/simpleFrag.frag.spv", pipelineConfig);
    }
    void Engine::createCommandBuffers() {
        commandBuffers.resize(swapchain->swapChainImages.size());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        // primary buffers can be submitted for execution but cannot be called by other buffers
        // secondary buffers cannot be submitted for execution directly but can be called by other buffers
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.commandPool;
        allocInfo.commandBufferCount = static_cast<unsigned int>(commandBuffers.size());
        if (vkAllocateCommandBuffers(device.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate command buffers!");
    }
    void Engine::recordCommandBuffer(const int& imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (swapchain->imagesInFlight[imageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(device.device, 1, &swapchain->imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("failed to begin recording command buffer!");
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapchain->renderPass;
        renderPassInfo.framebuffer = swapchain->swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {swapchain->swapChainExtent.width, swapchain->swapChainExtent.height};
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<unsigned int>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        // viewport, IMPORTANT, very useful for moving the viewport around and squishing it
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapchain->swapChainExtent.width);
        viewport.height = static_cast<float>(swapchain->swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        // scissor
        VkRect2D scissor{{0, 0}, swapchain->swapChainExtent};
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        pipeline->bind(commandBuffers[imageIndex]);
        for (Model*& model : models) {
            model->bind(commandBuffers[imageIndex]);
            model->draw(commandBuffers[imageIndex]);
        }

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
            throw std::runtime_error("failed to record command buffer!");
    }
    void Engine::drawFrame() {
        unsigned int imageIndex;
        VkResult result = swapchain->acquireNextImage(&imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapchain(); return;
        }
        if ((result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR))
            throw std::runtime_error("failed to aquire next swapchain image!");
            
        recordCommandBuffer(imageIndex);
        result = swapchain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR) || (window.frameBufferResized)) {
            window.frameBufferResized = false;
            recreateSwapchain(); return;
        }
        if (result != VK_SUCCESS)
            throw std::runtime_error("failed to present next swapchain image!");
    }
    void Engine::freeCommandBuffers() {
        vkFreeCommandBuffers(device.device, device.commandPool, static_cast<unsigned int>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }
}