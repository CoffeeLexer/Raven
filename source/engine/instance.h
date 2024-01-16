#pragma once

#ifdef __APPLE__
#include <vulkan/vulkan.h>
#else
#include <glad/vulkan.h>
#endif

namespace Instance
{
    void createInstance(VkInstance& instance);
};

void setupAppInfo(VkApplicationInfo& appInfo);
