#include "Engine.h"
#include <iostream>

namespace Eng {
    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
        : window(windowName, windowSize), device(&window),
        swapchain(&device, VkExtent2D{static_cast<unsigned int>(windowSize.x), static_cast<unsigned int>(windowSize.y)}), pipeline(nullptr)
    {
    }

    Engine::~Engine() {
        for (Model*& model : models)
            delete model;
        delete pipeline;
        vkDestroyPipelineLayout(device.device, pipelineLayout, nullptr);
        glfwTerminate();
    }
    void Engine::start() {
        createPipelineLayout();
        createPipeline();
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
    void Engine::createPipeline() {
        PipelineConfigInfo pipelineConfig = Pipeline::createDefaultConfig({swapchain.swapChainExtent.width, swapchain.swapChainExtent.height});
        pipelineConfig.renderPass = swapchain.renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = new Pipeline(&device, "out/shaders/simpleVert.vert.spv", "out/shaders/simpleFrag.frag.spv", pipelineConfig);
    }
    void Engine::createCommandBuffers() {
        commandBuffers.resize(swapchain.swapChainImages.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        // primary buffers can be submitted for execution but cannot be called by other buffers
        // secondary buffers cannot be submitted for execution directly but can be called by other buffers
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.commandPool;
        allocInfo.commandBufferCount = static_cast<unsigned int>(commandBuffers.size());
        if (vkAllocateCommandBuffers(device.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate command buffers!");

        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("failed to begin recording command buffer!");
            
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapchain.renderPass;
            renderPassInfo.framebuffer = swapchain.swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = {swapchain.swapChainExtent.width, swapchain.swapChainExtent.height};
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.025f, 0.0f, 0.025f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<unsigned int>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            pipeline->bind(commandBuffers[i]);
            for (Model*& model : models) {
                model->bind(commandBuffers[i]);
                model->draw(commandBuffers[i]);
            }

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to record command buffer!");
        }
    }
    void Engine::drawFrame() {
        unsigned int imageIndex;
        VkResult result = swapchain.acquireNextImage(&imageIndex);
        if ((result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR))
            throw std::runtime_error("failed to aquire next swapchain image!");
        result = swapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS)
            throw std::runtime_error("failed to present next swapchain image!");
    }
}