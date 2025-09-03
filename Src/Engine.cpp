#include "Engine.h"

namespace Eng {
    struct GlobalUniformBufferObject {
        mat4 projectionView{1.0f};
        vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.2f};
        vec3 lightPosition{0.0f, -1.0f, 1.0f};
        alignas(16) vec4 lightColor{1.0f, 0.0f, 1.0f, 1.0f};
    } uniformBufferElement;

    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
        : window(windowName, windowSize), device(&window), renderer(&window, &device)
    {   
        globalPool = DescriptorPool::Builder(&device)
            .setMaxSets(Swapchain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Swapchain::MAX_FRAMES_IN_FLIGHT)
            .build();
        meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/flat_vase.obj"));
        meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/smooth_vase.obj"));
        meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/suzanne.obj"));
        meshes.push_back(Loaders::meshFromObj(&device, "Resources/obj/Quad.obj"));
    }

    Engine::~Engine() {
        delete globalPool;
        for (Mesh* mesh : meshes)
            delete mesh;
    }
    void Engine::addObject(const vec3& position, const vec3& scale, const vec3& rotation, const unsigned int& meshIndex) {
        if (started) return;
        GameObject object = GameObject::createGameObject();
        object.mesh = meshes[meshIndex];
        object.transform.position = position;
        object.transform.scale = scale;
        object.transform.rotation = rotation;
        objects.emplace(object.id, (GameObject&&)object);
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
    void Engine::run() {
        // create uniform buffers
        std::vector<OwnedPointer<Buffer>> uniformBuffers;
        for (unsigned int i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
            uniformBuffers.emplace_back(new Buffer(&device, sizeof(GlobalUniformBufferObject), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, device.properties.limits.minUniformBufferOffsetAlignment));
            uniformBuffers[i]->map();
        }
        // create uniform buffer descriptor set layouts
        OwnedPointer<DescriptorSetLayout> globalDescriptorSetLayout = DescriptorSetLayout::Builder(&device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT).build();
        // create uniform buffer descriptor sets
        std::vector<VkDescriptorSet> globalDescriptorSets(Swapchain::MAX_FRAMES_IN_FLIGHT);
        for (unsigned int i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferDescriptor = uniformBuffers[i]->descriptorInfo();
            DescriptorWriter(globalDescriptorSetLayout.value, globalPool).writeBuffer(0, &bufferDescriptor).build(globalDescriptorSets[i]);
        }

        // setup rendering
        SimpleRenderSystem renderSystem(&device, renderer.getRenderPass(), globalDescriptorSetLayout->descriptorSetLayout);
        Camera camera;
        TransformComponent cameraTransform;
        cameraTransform.position.z = -2.5f;
        camera.setViewYXZ(cameraTransform.position, cameraTransform.rotation);
        FrameInfo frameInfo{ 0, 0, VK_NULL_HANDLE, &camera, VK_NULL_HANDLE, &objects };

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

            frameInfo.commandBuffer = renderer.beginFrame();
            if (frameInfo.commandBuffer != VK_NULL_HANDLE) {
                frameInfo.frameIndex = renderer.getFrame();
                frameInfo.dt = dt;
                frameInfo.globalDescriptorSet = globalDescriptorSets[frameInfo.frameIndex];
                update(frameInfo);
                uniformBuffers[frameInfo.frameIndex]->writeAtIndex(&uniformBufferElement, 0);
                uniformBuffers[frameInfo.frameIndex]->flushAtIndex(0);
                renderer.beginRenderPass(frameInfo.commandBuffer);
                renderSystem.recordObjects(frameInfo);
                renderer.endRenderPass(frameInfo.commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device);
    }
    void Engine::update(FrameInfo& frameInfo) {
        uniformBufferElement.projectionView = frameInfo.camera->projection*frameInfo.camera->view;
    }
}