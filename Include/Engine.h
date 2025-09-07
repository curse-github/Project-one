#ifndef ENG_ENGINE
#define ENG_ENGINE

#include "Helpers.h"
#include "Window.h"
#include "Device.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Mesh.h"
#include "GameObject.h"
#include "RenderSystems.h"
#include "Loaders.h"
#include "Buffer.h"
#include "Descriptors.h"
#include "FrameInfo.h"
#include "UboStructs.h"
#include "Texture.h"

namespace Eng {
    class Engine {
        KeyMappings keys{};
        float speed = 3.0f;
        float sensitivity = 1.5f;

        Window window;
        Device device;
        Renderer renderer;
        
        OwnedPointer<DescriptorPool> globalDescriptorPool;
        std::unordered_map<std::string, OwnedPointer<Mesh>> meshes;
        unsigned int maxTextures;
        std::vector<OwnedPointer<Texture>> textures;
        std::unordered_map<std::string, size_t> textureIndxs;
        GameObject::Map objects;
        GameObject::Map lights;
        
        bool started = false;
        bool pollMovement(const float& dt, TransformComponent& transform);
    public:
        Engine(const std::string& windowName, const ivec2& windowSize);
        Engine(const Engine& copy) = delete;
        Engine& operator=(const Engine& copy) = delete;
        Engine(Engine&& move) = delete;
        Engine& operator=(Engine&& move) = delete;
        ~Engine();

        GameObject::id_t addObject(const vec3& position, const vec3& scale, const vec3& rotation, const std::string& mesh, const std::string& texture, const vec3& specColor, const float& specExp);
        GameObject::id_t addLight(const vec3& position, const float& size, const vec3& color, const float& intensity);
        
        void start();
        void run();
        void update(FrameInfo& frameInfo);
    };
}

#endif