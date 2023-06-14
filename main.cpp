#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
    std::cout << "Start of execution\n";

    glfwInit();
    GLFWwindow *window = glfwCreateWindow(250, 250, "Window", nullptr, nullptr);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    
    std::cout << "End   of execution\n";

    return 0;
}