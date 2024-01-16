#pragma once

#ifdef __APPLE__
#include <vulkan/vulkan.h>
#else
#include <glad/vulkan.h>
#endif

namespace DebugMessenger
{
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    void setupDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    bool isValidationLayerSupported();
    void setupValidationLayers(VkInstanceCreateInfo& createInfo);
    void createDebugMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger);
};
