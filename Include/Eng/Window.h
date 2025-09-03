#ifndef ENG_WINDOW
#define ENG_WINDOW

#include "Helpers.h"

namespace Eng {
    class Window {
        GLFWwindow* window;

        static void frameBufferResizedCallback(GLFWwindow* glfwWindow, int width, int height);
    public:
        std::string name;
        ivec2 size;
        bool frameBufferResized;
        Window(const std::string& _name, const ivec2& _size);
        Window(const Window& copy) = delete;
        Window& operator=(const Window& copy) = delete;
        Window(Window&& move) = delete;
        Window& operator=(Window&& move) = delete;
        ~Window();

        bool shouldClose();
        bool createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        bool keyPressed(const int& key);
    };
}

#endif