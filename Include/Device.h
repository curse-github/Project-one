#ifndef __DEVICE
#define __DEVICE

#include "Helpers.h"
#include "Window.h"

namespace Eng {
    // https://drive.google.com/drive/folders/1Hs-3v_AFVbASmymY4I2UB-JWvW3-hTAV
    class Device {
        Window* window;
        bool enableValidationLayers = true;
        // vulkan things
        VkPhysicalDeviceProperties properties;
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        const std::vector<const char *> instanceExtensions = {"VK_NV_external_memory_capabilities"};
    public:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkDevice device;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        Device(Window* _window);
        Device(const Device& copy) = delete;
        Device& operator=(const Device& copy) = delete;
        Device(Device&& move) = delete;
        Device& operator=(Device&& move) = delete;
        ~Device();
        
        Eng::SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); };
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); };
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(VkBuffer buffer, VkImage image, unsigned int width, unsigned int height, unsigned int layerCount);
        void createImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);// used in swapchain
    private:

        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

        bool checkValidationLayerSupport();
        std::vector<const char *> getRequiredExtensions();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void hasGflwRequiredInstanceExtensions();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        // only used by buffer functions
        unsigned int findMemoryType(unsigned int typeFilter, VkMemoryPropertyFlags properties);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    };
}

#endif