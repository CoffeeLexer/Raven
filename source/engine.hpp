#pragma once

#ifdef __APPLE__
#include <vulkan/vulkan.h>
#else
#include <glad/vulkan.h>
#endif

#include <vector>
#include <optional>

struct GLFWwindow;

std::vector<char> readFile(const std::string& fileName);

const std::vector<const char*> validationLayers =
{
    "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_KHR_portability_subset",
};

const int MAX_FRAMES_IN_FLIGHT = 2;

struct QueueFamilyDetails
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    QueueFamilyDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    QueueFamilyDetails() = default;

    bool isComplete();
};

struct SwapChainDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    SwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    SwapChainDetails()  = default;

    VkExtent2D chooseSwapExtent(GLFWwindow* window);
    VkSurfaceFormatKHR chooseFormat();
    VkPresentModeKHR choosePresentMode();
};

class Engine
{

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    QueueFamilyDetails queueFamilyDetails;
    VkQueue graphicsQueue, presentQueue;
    VkDevice device;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    GLFWwindow* window;
    std::vector<const char*> requiredExtensions;
    uint32_t currentFrame = 0;

    void queueRequiredExtensions();
    bool isValidationLayerSupported();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    VkShaderModule createShaderModule(const std::vector<char>& source);

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();
    void destroySyncObjects();

    void createInstance();
    void createDebugMessenger();
    void createSurface();
    void queryPhysicalDevice();
    void createDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();

    void destroyInstance();
    void destroyDebugMessenger();
    void destroySurface();
    void destroyDevice();
    void destroySwapChain();
    void destroyImageViews();
    void destroyRenderPass();
    void destroyGraphicsPipeline();
    void destroyFramebuffers();
    void destroyCommandPool();

    // Not Required - Automatic deallocation
    // void destroyCommandBuffer();

public:
    void drawFrame();

    void create(GLFWwindow* win)
    {
        window = win;

        queueRequiredExtensions();

        createInstance();
        createDebugMessenger();
        createSurface();
        queryPhysicalDevice();
        createDevice();
        createSwapChain();
        createImageViews();
        createRenderPass();
        createGraphicsPipeline();
        createFramebuffers();
        createCommandPool();
        createCommandBuffer();
        createSyncObjects();
    }

    void destroy()
    {
        vkDeviceWaitIdle(device);

        destroySyncObjects();
        destroyCommandPool();
        destroyFramebuffers();
        destroyGraphicsPipeline();
        destroyRenderPass();
        destroyImageViews();
        destroySwapChain();
        destroyDevice();
        destroySurface();
        destroyDebugMessenger();
        destroyInstance();
    }
};
