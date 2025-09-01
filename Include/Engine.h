#ifndef __ENGINE
#define __ENGINE

#include "Helpers.h"
#include "Window.h"
#include "Device.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Model.h"
#include "GameObject.h"
#include "RenderSystem.h"
#include "Camera.h"

namespace Eng {
    class Engine {
        Window window;
        Device device;
        Renderer renderer;

        Model* model;
        std::vector<GameObject> objects;
        
        bool started = false;
    public:
        Engine(const std::string& windowName, const ivec2& windowSize);
        Engine(const Engine& copy) = delete;
        Engine& operator=(const Engine& copy) = delete;
        Engine(Engine&& move) = delete;
        Engine& operator=(Engine&& move) = delete;
        ~Engine();

        void addObject(const vec3& position, const vec3& scale, const vec3& rotation);
        void start();
        void loop();
    };
}

#endif