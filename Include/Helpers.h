#ifndef __HELPERS
#define __HELPERS
#define _DEBUG
#define VSYNC

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
using glm::ivec2;
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <set>
#include <unordered_set>
#include <cstring>
#include <array>

namespace Eng {
    #pragma region vulkan structs
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    struct QueueFamilyIndices {
        unsigned int graphicsFamily;
        unsigned int presentFamily;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };
    #pragma endregion vulkan structs

    std::vector<char> readFile(const std::string& path);
}

#endif