#include "Engine.h"

namespace Eng {
    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
    : window(windowName, windowSize) {

    }

    Engine::~Engine() {
        glfwTerminate();
    }
    void Engine::start() {
    }
    void Engine::loop() {
        while(!window.shouldClose()) {
            glfwPollEvents();
        }
    }
}