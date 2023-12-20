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
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    bool framebufferResized = false;
    GLFWwindow* window;
    std::vector<const char*> requiredExtensions;
    uint32_t currentFrame = 0;

    void queueRequiredExtensions();
    bool isValidationLayerSupported();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkShaderModule createShaderModule(const std::vector<char>& source);
    void updateUniformBuffer(uint32_t index);
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

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
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createCommandPool();
    void createDepthResource();
    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void createDescriptorSets();
    void createCommandBuffer();

    void destroyInstance();
    void destroyDebugMessenger();
    void destroySurface();
    void destroyDevice();
    void destroySwapChain();
    void destroyImageViews();
    void destroyDepthResources();
    void destroyRenderPass();
    void destroyDescriptorSetLayout();
    void destroyGraphicsPipeline();
    void destroyFramebuffers();
    void destroyUniformBuffers();
    void destroyDescriptorPool();
    void destroyDescriptorSets();
    void destroyVertexBuffer();
    void destroyIndexBuffer();
    void destroyCommandPool();

    // Not Required - Automatic deallocation
    // void destroyCommandBuffer();

    void recreateSwapChain();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);

public:
    void drawFrame();
    void setFramebufferResized(bool);

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
        createDescriptorSetLayout();
        createGraphicsPipeline();
        createCommandPool();
        createDepthResource();
        createFramebuffers();
        createTextureImage();
        createTextureImageView();
        createTextureSampler();
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffer();
        createSyncObjects();
    }

    void destroy()
    {
        vkDeviceWaitIdle(device);

        destroySyncObjects();
        destroyCommandPool();
        destroyDescriptorSets();
        destroyDescriptorPool();
        destroyUniformBuffers();
        destroyIndexBuffer();
        destroyVertexBuffer();
        destroyFramebuffers();
        destroyGraphicsPipeline();
        destroyDescriptorSetLayout();
        destroyRenderPass();
        destroyImageViews();
        destroySwapChain();
        destroyDepthResources();
        vkDestroySampler(device, textureSampler, nullptr);
        vkDestroyImageView(device, textureImageView, nullptr);
        vkDestroyImage(device, textureImage, nullptr);
        vkFreeMemory(device, textureImageMemory, nullptr);
        destroyDevice();
        destroySurface();
        destroyDebugMessenger();
        destroyInstance();
    }
};
