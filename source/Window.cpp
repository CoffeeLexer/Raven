#include "Window.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

Window::Window() {
    if(!glfwInit())
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
}

Window::~Window() {

}
