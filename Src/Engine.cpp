#include "Engine.h"

namespace Eng {
    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
        : window(windowName, windowSize), device(&window), renderer(&window, &device)
    {
        model = new Model(&device, {
            {{-1.0,  1.0}, {1.0f, 0.0f, 0.0f}},
            {{-1.0, -1.0}, {0.0f, 1.0f, 0.0f}},
            {{ 1.0, -1.0}, {0.0f, 0.0f, 1.0f}},
            {{ 1.0, -1.0}, {0.0f, 0.0f, 1.0f}},
            {{ 1.0,  1.0}, {1.0f, 1.0f, 1.0f}},
            {{-1.0,  1.0}, {1.0f, 0.0f, 0.0f}}
        });
    }

    Engine::~Engine() {
        delete model;
    }
    void Engine::start() {
        started = true;
    }
    void Engine::loop() {
        SimpleRenderSystem renderSystem(&device, renderer.getRenderPass());
        while(!window.shouldClose()) {
            glfwPollEvents();
            VkCommandBuffer commandBuffer = renderer.beginFrame();
            if (commandBuffer != VK_NULL_HANDLE) {
                renderer.beginRenderPass(commandBuffer);
                renderSystem.recordObjects(commandBuffer, objects);
                renderer.endRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device);
    }
    void Engine::addObject(const vec3& position, const vec3& scale, const float& rotation) {
        if (started) return;
        GameObject object = GameObject::createGameObject();
        object.model = model;
        object.transform.position = position;
        object.transform.scale = scale;
        object.transform.rotation = rotation;
        objects.push_back((GameObject&&)object);
    }
}