#ifndef __HELPERS
#define __HELPERS
#define _DEBUG
#define VSYNC

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
using glm::vec2;
using glm::ivec2;
#include <glm/vec3.hpp>
using glm::vec3;
#include <glm/vec4.hpp>
using glm::vec4;
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
using glm::mat4;
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <set>
#include <unordered_set>
#include <cstring>
#include <array>
#include <cassert>

namespace Eng {
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

    std::vector<char> readFile(const std::string& path);
}

#endif