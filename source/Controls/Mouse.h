//
// Created by tartarus on 11/23/22.
//

#ifndef RAVEN_MOUSE_H
#define RAVEN_MOUSE_H

#include <GLFW/glfw3.h>

namespace Raven {

    class Mouse {

        //  Action: 0-released, 1-pressed
        //  Button: 0-LMB, 1-RMB, 2-ScrollButton
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

        //  Middle button Scroll:
        //      Down: -1.000
        //      Up  : +1.000
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    public:
        Mouse(GLFWwindow*);
    };

} // Raven

#endif //RAVEN_MOUSE_H
