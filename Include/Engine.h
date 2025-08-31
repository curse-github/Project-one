#ifndef __ENGINE
#define __ENGINE

#include "Helpers.h"
#include "Window.h"
#include "Device.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "Model.h"

namespace Eng {
    class Engine {
        Window window;
        Device device;
        Swapchain* swapchain;
        Pipeline* pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<Model*> models;
        
        bool started = false;

        void createPipelineLayout();
        void recreateSwapchain();
        void recreatePipeline();
        void createCommandBuffers();
        void recordCommandBuffer(const int& imageIndex);
        void drawFrame();
        void freeCommandBuffers();
    public:
        Engine(const std::string& windowName, const ivec2& windowSize);
        Engine(const Engine& copy) = delete;
        Engine& operator=(const Engine& copy) = delete;
        Engine(Engine&& move) = delete;
        Engine& operator=(Engine&& move) = delete;
        ~Engine();

        void addModel(const std::vector<Model::Vertex>& vertices);
        void start();
        void loop();
    };
}

#endif