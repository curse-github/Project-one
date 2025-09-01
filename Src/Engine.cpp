#include "Engine.h"

namespace Eng {
    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
        : window(windowName, windowSize), device(&window), renderer(&window, &device)
    {
        model = new Model(&device, {
            // top, y=+1, yellow
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.80f, 0.10f}, { 0.0f,  1.0f,  0.0f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.80f, 0.80f, 0.10f}, {0.0f, 1.0f, 0.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.80f, 0.10f}, {0.0f, 1.0f, 0.0f}},
            {{-1.0f,  1.0f,  1.0f}, {0.80f, 0.80f, 0.10f}, {0.0f, 1.0f, 0.0f}},
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.80f, 0.10f}, {0.0f, 1.0f, 0.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.80f, 0.10f}, {0.0f, 1.0f, 0.0f}},
            // right, x=+1, red
            {{ 1.0f, -1.0f, -1.0f}, {0.80f, 0.10f, 0.10f}, { 1.0f,  0.0f,  0.0f}},
            {{ 1.0f, -1.0f,  1.0f}, {0.80f, 0.10f, 0.10f}, {1.0f, 0.0f, 0.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.10f, 0.10f}, {1.0f, 0.0f, 0.0f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.80f, 0.10f, 0.10f}, {1.0f, 0.0f, 0.0f}},
            {{ 1.0f, -1.0f, -1.0f}, {0.80f, 0.10f, 0.10f}, {1.0f, 0.0f, 0.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.80f, 0.10f, 0.10f}, {1.0f, 0.0f, 0.0f}},
            // front, z=-1, green
            {{-1.0f, -1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}, { 0.0f,  0.0f, -1.0f}},
            {{ 1.0f, -1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}, { 0.0f,  0.0f, -1.0f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}, { 0.0f,  0.0f, -1.0f}},
            {{-1.0f,  1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}, { 0.0f,  0.0f, -1.0f}},
            {{-1.0f, -1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}, { 0.0f,  0.0f, -1.0f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.10f, 0.80f, 0.10f}, { 0.0f,  0.0f, -1.0f}},
            // left, x=-1
            {{-1.0f, -1.0f,  1.0f}, {0.80f, 0.45f, 0.10f}, {-1.0f,  0.0f,  0.0f}},
            {{-1.0f, -1.0f, -1.0f}, {0.80f, 0.45f, 0.10f}, {-1.0f,  0.0f,  0.0f}},
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.45f, 0.10f}, {-1.0f,  0.0f,  0.0f}},
            {{-1.0f,  1.0f,  1.0f}, {0.80f, 0.45f, 0.10f}, {-1.0f,  0.0f,  0.0f}},
            {{-1.0f, -1.0f,  1.0f}, {0.80f, 0.45f, 0.10f}, {-1.0f,  0.0f,  0.0f}},
            {{-1.0f,  1.0f, -1.0f}, {0.80f, 0.45f, 0.10f}, {-1.0f,  0.0f,  0.0f}},
            // back, z=+1
            {{ 1.0f, -1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}, { 0.0f,  0.0f,  1.0f}},
            {{-1.0f, -1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}, { 0.0f,  0.0f,  1.0f}},
            {{-1.0f,  1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}, { 0.0f,  0.0f,  1.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}, { 0.0f,  0.0f,  1.0f}},
            {{ 1.0f, -1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}, { 0.0f,  0.0f,  1.0f}},
            {{-1.0f,  1.0f,  1.0f}, {0.10f, 0.10f, 0.80f}, { 0.0f,  0.0f,  1.0f}},
            // bottom, y=-1, white
            {{ 1.0f, -1.0f,  1.0f}, {1.00f, 1.00f, 1.00f}, { 0.0f, -1.0f,  0.0f}},
            {{ 1.0f, -1.0f, -1.0f}, {1.00f, 1.00f, 1.00f}, { 0.0f, -1.0f,  0.0f}},
            {{-1.0f, -1.0f, -1.0f}, {1.00f, 1.00f, 1.00f}, { 0.0f, -1.0f,  0.0f}},
            {{ 1.0f, -1.0f,  1.0f}, {1.00f, 1.00f, 1.00f}, { 0.0f, -1.0f,  0.0f}},
            {{-1.0f, -1.0f, -1.0f}, {1.00f, 1.00f, 1.00f}, { 0.0f, -1.0f,  0.0f}},
            {{-1.0f, -1.0f,  1.0f}, {1.00f, 1.00f, 1.00f}, { 0.0f, -1.0f,  0.0f}}
        });
    }

    Engine::~Engine() {
        delete model;
    }
    void Engine::start() {
        started = true;
    }
    bool Engine::pollMovement(const float& dt, TransformComponent& transform) {
        bool updated = false;
        vec3 rotation(0.0f, 0.0f, 0.0f);
        if (window.keyPressed(keys.lookRight)) rotation.y += 1.0f;
        if (window.keyPressed(keys.lookLeft)) rotation.y -= 1.0f;
        if (window.keyPressed(keys.lookUp)) rotation.x += 1.0f;
        if (window.keyPressed(keys.lookDown)) rotation.x -= 1.0f;
        if (glm::dot(rotation, rotation) > std::numeric_limits<float>::epsilon()){
            transform.rotation += sensitivity*dt*normalize(rotation);
            transform.rotation.x = glm::clamp(transform.rotation.x, -DEG90, DEG90);
            transform.rotation.y = glm::mod(transform.rotation.y, DEG360);
            updated = true;
        }
        const vec3 forward = vec3(glm::sin(transform.rotation.y), 0.0f, glm::cos(transform.rotation.y));
        const vec3 up = vec3(0.0f, -1.0f, 0.0f);
        const vec3 right = vec3(forward.z, 0.0f, -forward.x);// alternatively glm::cross(forward, up)
        vec3 movement(0.0f, 0.0f, 0.0f);
        if (window.keyPressed(keys.moveForward)) movement += forward;
        if (window.keyPressed(keys.moveBackward)) movement -= forward;
        if (window.keyPressed(keys.moveRight)) movement += right;
        if (window.keyPressed(keys.moveLeft)) movement -= right;
        if (window.keyPressed(keys.moveUp)) movement += up;
        if (window.keyPressed(keys.moveDown)) movement -= up;
        if (glm::dot(movement, movement) > std::numeric_limits<float>::epsilon()) {
            transform.position += speed*dt*normalize(movement); updated = true;
        }
        return updated;
    }
    void Engine::loop() {
        SimpleRenderSystem renderSystem(&device, renderer.getRenderPass());
        Camera camera;
        TransformComponent cameraTransform;
        // camera.setViewDirection(vec3(0.0f), vec3(0.5f, 0.0f, 1.0f));
        // camera.setViewTarget(vec3(-1.0f, -2.0f, -1.0f), vec3(0.0f, 0.0f, 2.5f));
        camera.setViewYXZ(cameraTransform.position, cameraTransform.rotation);
        std::chrono::_V2::system_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        while(!window.shouldClose()) {
            glfwPollEvents();
            std::chrono::_V2::system_clock::time_point newTime = std::chrono::high_resolution_clock::now();
            float dt = std::chrono::duration<float, std::chrono::seconds::period>(newTime-currentTime).count();
            currentTime = newTime;
            std::cout << "fps: " << (1/dt) << '\n';
            if (pollMovement(glm::min(dt, 1.0f/30.0f), cameraTransform))
                camera.setViewYXZ(cameraTransform.position, cameraTransform.rotation);
            camera.setProj(glm::radians(50.0f), renderer.getAspectRatio(), 0.1f, 100.0f);// camera.setOrtho(-aspect, aspect, 1, -1, -1, 1);
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