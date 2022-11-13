
#ifndef RAVEN_GLFWCALLBACKS_H
#define RAVEN_GLFWCALLBACKS_H


#include "GLFW/glfw3.h"
namespace Raven {
    class glfwCallbacks {
    public:
        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}

#endif //RAVEN_GLFWCALLBACKS_H
