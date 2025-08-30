#ifndef __ENGINE
#define __ENGINE

#include "Helpers.h"
#include "Window.h"
#include "Device.h"
#include "Pipeline.h"
#include "Swapchain.h"

namespace Eng {
    class Engine {
        Window window;
        Device device;
        Swapchain swapchain;
        Pipeline* pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;

        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
    public:
        Engine(const std::string& windowName, const ivec2& windowSize);
        Engine(const Engine& copy) = delete;
        Engine& operator=(const Engine& copy) = delete;
        Engine(Engine&& move) = delete;
        Engine& operator=(Engine&& move) = delete;
        ~Engine();

        void start();
        void loop();
    };
}

#endif