#ifndef __ENGINE
#define __ENGINE

#include "Helpers.h"
#include "Window.h"
#include "Device.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Mesh.h"
#include "GameObject.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "Loaders.h"

namespace Eng {
    struct KeyMappings {
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_SPACE;
        int moveDown = GLFW_KEY_LEFT_SHIFT;
        int lookLeft = GLFW_KEY_LEFT;
        int lookRight = GLFW_KEY_RIGHT;
        int lookUp = GLFW_KEY_UP;
        int lookDown = GLFW_KEY_DOWN;
    };
    class Engine {
        Window window;
        Device device;
        Renderer renderer;
        
        KeyMappings keys{};
        float speed = 3.0f;
        float sensitivity = 1.5f;

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