#ifdef __APPLE__
#include <vulkan/vulkan.h>
#else
#include <glad/vulkan.h>
#endif

#include <GLFW/glfw3.h>

#include <iostream>

#include "source/engine.hpp"

namespace options {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
}

int main()
{
    std::cout << "Start of execution" << std::endl;

    // INITIALIZE
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow *window;

    window = glfwCreateWindow(options::WIDTH, options::HEIGHT, "Window", nullptr, nullptr);

    Engine engine;

    engine.create(window);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        engine.drawFrame();
    }

    engine.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "End   of execution" << std::endl;
    return 0;
}
