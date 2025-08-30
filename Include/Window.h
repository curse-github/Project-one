#ifndef __WINDOW
#define __WINDOW

#include "Helpers.h"

namespace Eng {
    class Window {
        GLFWwindow* window;
    public:
        std::string name;
        ivec2 size;
        Window(const std::string& _name, const ivec2& _size);
        Window(const Window& copy) = delete;
        Window& operator=(const Window& copy) = delete;
        Window(Window&& move) = delete;
        Window& operator=(Window&& move) = delete;
        ~Window();
        bool shouldClose();
        bool createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    };
}

#endif