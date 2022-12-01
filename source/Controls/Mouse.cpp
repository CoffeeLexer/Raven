//
// Created by tartarus on 11/23/22.
//

#include "Mouse.h"

#include <GLFW/glfw3.h>

namespace Raven {

    Mouse::Mouse(GLFWwindow* window) {
        glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
        glfwSetScrollCallback(window, Mouse::scroll_callback);
    }

    void Mouse::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
#ifdef LOG_MOUSE_BUTTON
        printf("MOUSE::BUTTON\n\tButton: %i, Action: %i, Mods: %i\n", button, action, mods);
#endif
    }

    void Mouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
#ifdef LOG_MOUSE_SCROLL
        printf("MOUSE::SCROLL\n\tX: %.3f, Y: %.3f\n", xoffset, yoffset);
#endif
    }

} // Raven

