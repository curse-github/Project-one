#include "Engine.h"

namespace Eng {
    struct GlobalUniformBufferObject {
        glm::mat4 projectionView{1.0f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0, -3.0, -1.0));
    } uniformBufferElement;

    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
        : window(windowName, windowSize), device(&window), renderer(&window, &device)
    {   
        globalPool = DescriptorPool::Builder(&device)
            .setMaxSets(Swapchain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Swapchain::MAX_FRAMES_IN_FLIGHT)
            .build();
        // meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/CubeWhite.obj"));
        // meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/CubeRubix.obj"));
        meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/flat_vase.obj"));
        meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/smooth_vase.obj"));
    }

    Engine::~Engine() {
        delete globalPool;
        for (Mesh* mesh : meshes)
            delete mesh;
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
        const vec3 right = vec3(forward.z, 0.0f, -forward.x);// alternatively glm::cross(forward, up)
        vec3 movement(0.0f, 0.0f, 0.0f);
        if (window.keyPressed(keys.moveForward)) movement += forward;
        if (window.keyPressed(keys.moveBackward)) movement -= forward;
        if (window.keyPressed(keys.moveRight)) movement += right;
        if (window.keyPressed(keys.moveLeft)) movement -= right;
        if (window.keyPressed(keys.moveUp)) movement.y -= 1;
        if (window.keyPressed(keys.moveDown)) movement.y += 1;
        if (glm::dot(movement, movement) > std::numeric_limits<float>::epsilon()) {
            transform.position += speed*dt*normalize(movement); updated = true;
        }
        return updated;
    }
    std::chrono::_V2::system_clock::time_point lastPrint = std::chrono::high_resolution_clock::now();
    unsigned int frames = 0;
    void Engine::loop() {
        // setup uniform buffers
        std::vector<OwnedPointer<Buffer>> uniformBuffers;
        for (unsigned int i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
            uniformBuffers.emplace_back(OwnedPointer(new Buffer(&device, sizeof(GlobalUniformBufferObject), 1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                device.properties.limits.minUniformBufferOffsetAlignment)));
            uniformBuffers[i]->map();
        }
        OwnedPointer<DescriptorSetLayout> globalDescriptorSetLayout = DescriptorSetLayout::Builder(&device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();
        std::vector<VkDescriptorSet> globalDescriptorSets(Swapchain::MAX_FRAMES_IN_FLIGHT);
        for (unsigned int i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferDescriptor = uniformBuffers[i]->descriptorInfo();
            DescriptorWriter(globalDescriptorSetLayout.value, globalPool)
                .writeBuffer(0, &bufferDescriptor)
                .build(globalDescriptorSets[i]);
        }

        // setup rendering
        SimpleRenderSystem renderSystem(&device, renderer.getRenderPass(), globalDescriptorSetLayout->descriptorSetLayout);
        Camera camera;
        TransformComponent cameraTransform;
        camera.setViewYXZ(cameraTransform.position, cameraTransform.rotation);

        std::chrono::_V2::system_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        while(!window.shouldClose()) {
            glfwPollEvents();
            std::chrono::_V2::system_clock::time_point newTime = std::chrono::high_resolution_clock::now();
            float dt = std::chrono::duration<float, std::chrono::seconds::period>(newTime-currentTime).count();
            currentTime = newTime;
            // calc fps
            frames++;
            if (std::chrono::duration<float, std::chrono::seconds::period>(currentTime-lastPrint).count() > 2) {
                std::cout << "fps: " << (frames/2.0f) << '\n';
                frames=0; lastPrint = currentTime;
            }

            if (pollMovement(glm::min(dt, 1.0f/30.0f), cameraTransform)) camera.setViewYXZ(cameraTransform.position, cameraTransform.rotation);
            camera.setProj(glm::radians(50.0f), renderer.getAspectRatio(), 0.1f, 100.0f);// camera.setOrtho(-aspect, aspect, 1, -1, -1, 1);

            VkCommandBuffer commandBuffer = renderer.beginFrame();
            if (commandBuffer != VK_NULL_HANDLE) {
                const unsigned int frame = renderer.getFrame();
                FrameInfo frameInfo{ frame, dt, commandBuffer, &camera, globalDescriptorSets[frame] };
                uniformBufferElement.projectionView = camera.projection*camera.view;
                uniformBuffers[frameInfo.index]->writeAtIndex(&uniformBufferElement, 0);
                uniformBuffers[frameInfo.index]->flushAtIndex(0);

                renderer.beginRenderPass(commandBuffer);
                for (GameObject& object : objects)
                    object.transform.rotation.y += dt;
                renderSystem.recordObjects(frameInfo, objects);
                renderer.endRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device);
    }
    void Engine::addObject(const vec3& position, const vec3& scale, const vec3& rotation, const unsigned int& meshIndex) {
        if (started) return;
        GameObject object = GameObject::createGameObject();
        object.mesh = meshes[meshIndex];
        object.transform.position = position;
        object.transform.scale = scale;
        object.transform.rotation = rotation;
        objects.push_back((GameObject&&)object);
    }
}