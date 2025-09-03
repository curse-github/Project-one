#ifndef ENG_ENGINE
#define ENG_ENGINE

#include "Helpers.h"
#include "Window.h"
#include "Device.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Mesh.h"
#include "GameObject.h"
#include "RenderSystem.h"
#include "Loaders.h"
#include "Buffer.h"
#include "Descriptors.h"

namespace Eng {
    class Engine {
        KeyMappings keys{};
        float speed = 3.0f;
        float sensitivity = 1.5f;

        Window window;
        Device device;
        Renderer renderer;
        
        DescriptorPool* globalPool;
        std::vector<Mesh*> meshes;
        std::vector<GameObject> objects;
        
        bool started = false;
        bool pollMovement(const float& dt, TransformComponent& transform);
    public:
        Engine(const std::string& windowName, const ivec2& windowSize);
        Engine(const Engine& copy) = delete;
        Engine& operator=(const Engine& copy) = delete;
        Engine(Engine&& move) = delete;
        Engine& operator=(Engine&& move) = delete;
        ~Engine();

        void addObject(const vec3& position, const vec3& scale, const vec3& rotation, const unsigned int& meshIndex);
        void start();
        void loop();
    };
}

#endif