#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
    std::cout << "Start of execution\n";

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(250, 250, "Window", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(25.0f / 256.0f, 36.0f / 256.0f, 52.0f / 256.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    std::cout << "End   of execution\n";

    return 0;
}