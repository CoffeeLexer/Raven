#include "../engine.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            std::cout << "VALIDATION: " << pCallbackData->pMessage << std::endl;
            break;
        default:
            std::cerr << "VALIDATION: " << pCallbackData->pMessage << std::endl;
            break;
    }

    return VK_FALSE;
}

void Engine::queueRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtension;

    glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (size_t i = 0; i < glfwExtensionCount; i++)
    {
        requiredExtensions.push_back(glfwExtension[i]);
    }

#ifndef NDEBUG
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

#ifdef __APPLE__
    requiredExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    requiredExtensions.push_back("VK_KHR_get_physical_device_properties2");
#endif
}

static uint32_t ratePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyDetails queueFamilyIndices{device, surface};

    if (!queueFamilyIndices.isComplete())
        return 0;

    uint32_t score = 1;

    return score;
}

void Engine::createInstance()
{
#ifndef NDEBUG
    assert(isValidationLayerSupported());
#endif

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Application Name";
    appInfo.pEngineName = "No Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion =  VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#ifdef __APPLE__
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    createInfo.enabledExtensionCount = requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

#ifndef NDEBUG
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    populateDebugMessengerCreateInfo(debugCreateInfo);

    createInfo.pNext = &debugCreateInfo;
#else
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
#endif

    if (VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
            result != VK_SUCCESS)
    {
        std::cerr << "GFX::INSTANCE CREATE FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::createDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func == nullptr)
    {
        std::cerr << "DEBUG_MESSENGER::CREATE::NO EXTENSION" << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    if (VkResult result = func(instance, &createInfo, nullptr, &debugMessenger); result != VK_SUCCESS)
    {
        std::cerr << "DEBUG_MESSENGER::CREATE FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::createSurface()
{
    if (VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface); result)
    {
        std::cerr << "SURFACE CREATE FAILED: Uh oh" << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::queryPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (!deviceCount)
    {
        std::cerr << "PHYSICAL DEVICE QUERY No suitable device found!" << std::endl;
        throw std::runtime_error("CRASH");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::pair<uint32_t, VkPhysicalDevice> nominee = {0, nullptr};
    for (const auto& candidate : devices)
    {
        uint32_t score = ratePhysicalDevice(candidate, surface);
        if (score > nominee.first)
        {
            nominee = {score, candidate};
        }
    }

    if (nominee.first == 0)
    {
        std::cerr << "PHYSICAL DEVICE::CREATE No suitable device found!" << std::endl;
        throw std::runtime_error("CRASH");
    }

    physicalDevice = nominee.second;
}

void Engine::createDevice()
{
    QueueFamilyDetails indices{physicalDevice, surface};

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifndef NDEBUG
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    if (VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device); result != VK_SUCCESS)
    {
        std::cerr << "DEVICE CREATE Failed: " << result << std::endl;
        throw std::runtime_error("CRASH");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);

    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void Engine::createSwapChain()
{
    SwapChainDetails details{physicalDevice, surface};

    VkSurfaceFormatKHR format = details.chooseFormat();
    VkPresentModeKHR presentMode = details.choosePresentMode();
    VkExtent2D extent = details.chooseSwapExtent(window);

    uint32_t imageCount = details.capabilities.minImageCount + 1;
    if (!details.capabilities.maxImageCount)
    {
        imageCount = std::min(imageCount, details.capabilities.maxImageCount);
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyDetails indices{physicalDevice, surface};

    if (indices.graphicsFamily == indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    else
    {
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain); result)
    {
        std::cerr << "SWAP_CHAIN::CREATE FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = format.format;
    swapChainExtent = extent;
}

void Engine::destroyInstance()
{
    vkDestroyInstance(instance, nullptr);
}

void Engine::destroyDebugMessenger()
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func == nullptr)
    {
        std::cerr << "DEBUG_MESSENGER::DESTROY::NO EXTENSION" << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    func(instance, debugMessenger, nullptr);
}

void Engine::destroySurface()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

void Engine::destroyDevice()
{
    vkDestroyDevice(device, nullptr);
}

void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}

bool Engine::isValidationLayerSupported()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void Engine::destroySwapChain()
{
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void Engine::createImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (VkResult result = vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]); result)
        {
            std::cerr << "IMAGE VIEW CREATE FAILED: " << result << std::endl;
            throw std::runtime_error("FATAL CRASH");
        }
    }
}

void Engine::destroyImageViews()
{
    for (const auto& imageView : swapChainImageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
    }
}

VkShaderModule Engine::createShaderModule(const std::vector<char>& source)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

    VkShaderModule shaderModule;
    if (VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule); result)
    {
        std::cerr << "SHADER MODULE CREATE FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL ERROR!");
    }

    return shaderModule;
}

void Engine::createGraphicsPipeline()
{
    auto vertSource = readFile("shader/binary/vert.spv");
    auto fragSource = readFile("shader/binary/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertSource);
    VkShaderModule fragShaderModule = createShaderModule(fragSource);

    VkPipelineShaderStageCreateInfo vertexCreateInfo{};
    vertexCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexCreateInfo.module = vertShaderModule;
    vertexCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentCreateInfo{};
    fragmentCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentCreateInfo.module = fragShaderModule;
    fragmentCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo stages[] = {
        vertexCreateInfo, fragmentCreateInfo
    };

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<unsigned int>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    
    // Primitive assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    // Viewport area
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor Area
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;

    // Viewport/Scissor Creation
    VkPipelineViewportStateCreateInfo viewportInfo{};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &viewport;
    viewportInfo.pScissors = &scissor;
    viewportInfo.scissorCount = 1;

    // Rasterizer - Depth, Culling, Polygon mode, Line width
    VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizerInfo.depthBiasEnable = VK_FALSE;

    // Multisampler
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.sampleShadingEnable = VK_FALSE;
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Color Blend
    VkPipelineColorBlendAttachmentState colorBlend{};
    colorBlend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlend.blendEnable = VK_FALSE;
    
    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlend;
    
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    
    if (VkResult result = vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout); result != VK_SUCCESS)
    {
        std::cerr << "CREATE PIPELINE LAYOUT: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
    
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stages;
    
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &rasterizerInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pColorBlendState = &colorBlendInfo;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    
    if (VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline); result != VK_SUCCESS)
    {
        std::cerr << "CREATE GRAPHICS PIPELINE: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void Engine::destroyGraphicsPipeline()
{
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
}

void Engine::createRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;


    if (VkResult result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass); result != VK_SUCCESS)
    {
        std::cerr << "CREATE RENDER PASS: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::destroyRenderPass()
{
    vkDestroyRenderPass(device, renderPass, nullptr);
}

void Engine::createFramebuffers()
{
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
    {
        VkImageView attachments[] = {
            swapChainImageViews[i],
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.layers = 1;


        if (VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]); result != VK_SUCCESS)
        {
            std::cerr << "CREATE FRAMEBUFFER: " << result << std::endl;
            throw std::runtime_error("FATAL CRASH");
        }
    }
}

void Engine::destroyFramebuffers()
{
    for (auto& framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}

void Engine::createCommandPool()
{
    QueueFamilyDetails queueFamilyDetails{physicalDevice, surface};

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyDetails.graphicsFamily.value();

    if (VkResult result = vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool); result != VK_SUCCESS)
    {
        std::cerr << "CREATE COMMAND POOL: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::destroyCommandPool()
{
    vkDestroyCommandPool(device, commandPool, nullptr);
}

void Engine::createCommandBuffer()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (VkResult result = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer); result != VK_SUCCESS)
    {
        std::cerr << "ALLOC COMMAND BUFFER: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo); result != VK_SUCCESS)
    {
        std::cerr << "BEGIN COMMAND BUFFER: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor{{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChainExtent.width);
    viewport.height = static_cast<float>(swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;

    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS)
    {
        std::cerr << "END COMMAND BUFFER: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::createSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (VkResult result = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore); result != VK_SUCCESS)
    {
        std::cerr << "CREATE IMAGE SEMAPHORE FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    if (VkResult result = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore); result != VK_SUCCESS)
    {
        std::cerr << "CREATE RENDER SEMAPHORE FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    if (VkResult result = vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence); result != VK_SUCCESS)
    {
        std::cerr << "CREATE FENCE FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }
}

void Engine::destroySyncObjects()
{
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);
}

void Engine::drawFrame()
{
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFence);

    vkResetCommandBuffer(commandBuffer, 0);
    recordCommandBuffer(commandBuffer, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (VkResult result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence); result != VK_SUCCESS)
    {
        std::cerr << "SUBMIT COMMANDS FAILED: " << result << std::endl;
        throw std::runtime_error("FATAL CRASH");
    }

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;

    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(graphicsQueue, &presentInfo);
}
