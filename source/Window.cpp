#include "Window.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace Raven {

    const std::unordered_map<int, int> Window::sDefaultHints = {
#ifdef DEBUG
            {GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE},
            {GLFW_CONTEXT_NO_ERROR, GLFW_FALSE},
#else
            {GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE},
            {GLFW_CONTEXT_NO_ERROR, GLFW_FALSE},
#endif
            {GLFW_FLOATING, GLFW_TRUE},
            {GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE},
            {GLFW_DOUBLEBUFFER, GLFW_TRUE},
            {GLFW_SAMPLES, 4},
            {GLFW_CONTEXT_VERSION_MAJOR, 4},
            {GLFW_CONTEXT_VERSION_MINOR, 5},
            {GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE}, // MAC OS
            {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE},
    };

    Window::Window(const std::string& title, int width, int height) {
        if(!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW\n" );
            exit(1);
        }
        mHints = std::unordered_map<int, int>(sDefaultHints);
        mTitle = title;
        mWidth = width;
        mHeight = height;

        mWindow = nullptr;
    }

    Window::~Window() = default;

    void Window::setHint(int hint, int value) {
        mHints.emplace(hint, value);
    }

    void Window::setTitle(const std::string &title) {
        mTitle = title;
        if(mWindow != nullptr)
        {
            glfwSetWindowTitle(mWindow, mTitle.c_str());
        }
    }

     void Window::create() {
         mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
         if (mWindow == nullptr)
         {
             fprintf(stderr, "Failed to create GLFW window\n");
             glfwTerminate();
             exit(1);
         }
    }

    void Window::setContextCurrent() {
        if(mWindow != nullptr) glfwMakeContextCurrent(mWindow);
        else fprintf(stderr, "WINDOW::SET_CONTEXT_CURRENT\n\tWindow is not initialized!\n");
    }

    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        //glViewport(0, 0, width, height);
    }
}
