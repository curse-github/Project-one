#ifndef __ENGINE
#define __ENGINE

#include "Helpers.h"
#include "Window.h"

namespace Eng {
    class Engine {
        Window window;
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