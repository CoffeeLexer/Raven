//
// Created by tartarus on 11/5/22.
//

#ifndef RAVEN_WINDOW_H
#define RAVEN_WINDOW_H

#include <unordered_map>
#include <string>
#include <functional>
#include <GLFW/glfw3.h>
#include "UserPointer.h"

namespace Raven {
    class UserPointer;
    class Window {

        static const std::unordered_map<int, int> sDefaultHints;
        std::unordered_map<int, int> mHints;
        std::string mTitle;
        int mWidth, mHeight;
        GLFWwindow *mWindow;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    public:

        Window(const std::string& title, int width, int height);
        ~Window();
        void setHint(int hint, int value);
        void setTitle(const std::string &title);
        void create();
        void setContextCurrent();
        GLFWwindow* ptr() {return mWindow;}
    };
}

#endif //RAVEN_WINDOW_H
