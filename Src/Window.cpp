#include "Window.h"

namespace Eng {
    Window::Window(const std::string& _name, const ivec2& _size) : name(_name), size(_size){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(size.x, size.y, name.c_str(), nullptr, nullptr);// last argument is for fullscreen.
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
    }
    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }



    
    void Window::frameBufferResizedCallback(GLFWwindow* glfwWindow, int width, int height) {
        Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->frameBufferResized = true;
        window->size = {width, height};
    }
    
    bool Window::shouldClose() {
        return glfwWindowShouldClose(window);
    }
    bool Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
        return true;
    }
}