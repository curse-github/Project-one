#include "Engine.h"

namespace Eng {
    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
        : window(windowName, windowSize), device(&window), renderer(&window, &device)
    {
        model = new Model(&device, {
            // top, y=+1, yellow
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.80f, 0.10f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.80f, 0.80f, 0.10f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.80f, 0.10f}},
            {{-1.0f,  1.0f,  1.0f}, {0.80f, 0.80f, 0.10f}},
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.80f, 0.10f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.80f, 0.10f}},
            // right, x=+1, red
            {{ 1.0f, -1.0f, -1.0f}, {0.80f, 0.10f, 0.10f}},
            {{ 1.0f, -1.0f,  1.0f}, {0.80f, 0.10f, 0.10f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.10f, 0.10f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.80f, 0.10f, 0.10f}},
            {{ 1.0f, -1.0f, -1.0f}, {0.80f, 0.10f, 0.10f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.10f, 0.10f}},
            // front, z=-1, green
            {{-1.0f, -1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}},
            {{ 1.0f, -1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}},
        
            {{-1.0f,  1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}},
            {{-1.0f, -1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}},
            // left, x=-1
            {{-1.0f, -1.0f,  1.0f}, {0.80f, 0.45f, 0.10f}},
            {{-1.0f, -1.0f, -1.0f}, {0.80f, 0.45f, 0.10f}},
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.45f, 0.10f}},
            {{-1.0f,  1.0f,  1.0f}, {0.80f, 0.45f, 0.10f}},
            {{-1.0f, -1.0f,  1.0f}, {0.80f, 0.45f, 0.10f}},
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.45f, 0.10f}},
            // back, z=+1
            {{ 1.0f, -1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}},
            {{-1.0f, -1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}},
            {{-1.0f,  1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}},
            {{ 1.0f, -1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}},
            {{-1.0f,  1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}},
            // bottom, y=-1, white
            {{ 1.0f, -1.0f,  1.0f}, {1.00f, 1.00f, 1.00f}},
            {{ 1.0f, -1.0f, -1.0f}, {1.00f, 1.00f, 1.00f}},
            {{-1.0f, -1.0f, -1.0f}, {1.00f, 1.00f, 1.00f}},
            {{ 1.0f, -1.0f,  1.0f}, {1.00f, 1.00f, 1.00f}},
            {{-1.0f, -1.0f, -1.0f}, {1.00f, 1.00f, 1.00f}},
            {{-1.0f, -1.0f,  1.0f}, {1.00f, 1.00f, 1.00f}}
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
        Camera camera;
        while(!window.shouldClose()) {
            glfwPollEvents();
            float aspect = renderer.getAspectRatio();
            // camera.setOrtho(-aspect, aspect, 1, -1, -1, 1);
            camera.setProj(glm::radians(50.0f), aspect, 0.1f, 100.0f);
            VkCommandBuffer commandBuffer = renderer.beginFrame();
            if (commandBuffer != VK_NULL_HANDLE) {
                renderer.beginRenderPass(commandBuffer);
                renderSystem.recordObjects(commandBuffer, objects, camera);
                renderer.endRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device);
    }
    void Engine::addObject(const vec3& position, const vec3& scale, const vec3& rotation) {
        if (started) return;
        GameObject object = GameObject::createGameObject();
        object.model = model;
        object.transform.position = position;
        object.transform.scale = scale;
        object.transform.rotation = rotation;
        objects.push_back((GameObject&&)object);
    }
}