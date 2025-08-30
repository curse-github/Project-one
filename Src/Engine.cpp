#include "Engine.h"
#include <iostream>

namespace Eng {
    Engine::Engine(const std::string& windowName, const ivec2& windowSize)
    : window(windowName, windowSize), device(&window),
    pipeline(&device, "out/shaders/simpleVert.vert.spv", "out/shaders/simpleFrag.frag.spv", Pipeline::createDefaultConfig(windowSize)) {

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