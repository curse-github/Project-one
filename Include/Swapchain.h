#ifndef __SWAPCHAIN
#define __SWAPCHAIN

#include "Helpers.h"
#include "Device.h"

namespace Eng {
    // https://drive.google.com/drive/folders/1X0wUwujKw8OH2Z18OENadX0XBKWfU50w
    class Swapchain {
        Device* device;
        VkExtent2D windowExtent;
        VkSwapchainKHR swapChain;
        Swapchain* oldSwapchain;
    public:
        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        size_t currentFrame = 0;
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;
        std::vector<VkImageView> swapChainImageViews;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        Swapchain(Device* _device, VkExtent2D extent);
        Swapchain(Device* _device, VkExtent2D extent, Swapchain* previousSwapchain);
        Swapchain(const Swapchain& copy) = delete;
        Swapchain& operator=(const Swapchain& copy) = delete;
        Swapchain(Swapchain&& move) = delete;
        Swapchain& operator=(Swapchain&& move) = delete;
        ~Swapchain();

        VkResult acquireNextImage(unsigned int* imageIndex);
        VkResult submitCommandBuffers(const VkCommandBuffer* buffers, unsigned int* imageIndex);
    private:
        void init();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createDepthResources();
        void createFramebuffers();
        void createSyncObjects();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkFormat findDepthFormat();

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
    public:
        std::vector<VkFence> imagesInFlight;
    };
}

#endif