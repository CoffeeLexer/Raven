//
// Created by tartarus on 11/5/22.
//

#include "glfwCallbacks.h"

#include <cstdio>

void glfwCallbacks::error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void glfwCallbacks::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
