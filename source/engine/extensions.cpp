#include "extensions.h"

#ifdef __APPLE__
#include <vulkan/vulkan.h>
#else
#include <glad/vulkan.h>
#endif

#include "GLFW/glfw3.h"

#include <optional>

namespace Extensions
{
    std::vector<const char*> getRequired()
    {
        std::vector<const char*> extensions;

        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (uint32_t i = 0; i < glfwExtensionCount; i++)
        {
            extensions.push_back(glfwExtensions[i]);
        }

#ifndef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

#ifdef __APPLE__
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back("VK_KHR_get_physical_device_properties2");
#endif
        return extensions;
    }
}
