#ifdef __APPLE__
#include <vulkan/vulkan.h>
#else
#include <glad/vulkan.h>
#endif

#include <GLFW/glfw3.h>

#include <iostream>

#include "source/engine.hpp"
#include "source/example/example.hpp"

namespace options {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
}

void framebufferResizeCallback(GLFWwindow*, int, int);

int main()
{
    std::cout << "Start of execution" << std::endl;

    // INITIALIZE
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow *window;
    Engine engine;

    window = glfwCreateWindow(options::WIDTH, options::HEIGHT, "Window", nullptr, nullptr);

    glfwSetWindowUserPointer(window, &engine);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

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

void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    app->setFramebufferResized(true);
}