//
// Created by tartarus on 11/24/22.
//

#ifndef RAVEN_CALLBACKS_H
#define RAVEN_CALLBACKS_H

#include <vector>
#include <unordered_map>
#include <functional>
#include <GLFW/glfw3.h>

#include "Callback.h"

namespace Raven {
    class Callbacks {
    public:
        enum class TypesGlobal {
            Error,
            Joystick,
            Monitor,
        };
        enum class Types {
            Scroll,         Key,
            Char,           Drop,
            CursorEnter,    CursorPosition,
            MouseButton,    CharMods,
            FramebufferSize,WindowClose,
            WindowFocus,    WindowIconify,
            WindowMaximize, WindowPosition,
            WindowRefresh,  WindowSize,
            WindowContentScale};
    private:
        std::unordered_map<Types, void*> mDictionary;
        static Callbacks* ptr;

        //  Global callbacks
        static Callback<GLFWerrorfun> errorCallbacks;
        static Callback<GLFWjoystickfun> joystickCallbacks;
        static Callback<GLFWmonitorfun> monitorCallbacks;

    public:
        Callbacks();

        template<typename T>
        void append(Types key, std::function<std::remove_pointer_t<T>> callback);

        template<typename T>
        static void appendGlobal(TypesGlobal key, std::function<std::remove_pointer_t<T>> callback);

        void bind(GLFWwindow *);

        template<typename T, typename ...Args>
        void invoke(Types key, Args &&... args);

        template<TypesGlobal T, typename ...Args>
        static void invokeGlobal(Args &&...args);

        //  Callback linkers
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void char_callback(GLFWwindow* window, unsigned int codepoint);
        static void drop_callback(GLFWwindow* window, int path_count, const char* paths[]);
        static void error_callback(int error_code, const char* description);
        static void joystick_callback(int jid, int event);
        static void monitor_callback(GLFWmonitor* monitor, int event);
        static void cursor_enter_callback(GLFWwindow* window, int entered);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void window_close_callback(GLFWwindow* window);
        static void window_focus_callback(GLFWwindow* window, int focused);
        static void window_iconify_callback(GLFWwindow* window, int iconified);
        static void window_maximize_callback(GLFWwindow* window, int maximized);
        static void window_position_callback(GLFWwindow* window, int xpos, int ypos);
        static void window_refresh_callback(GLFWwindow* window);
        static void window_size_callback(GLFWwindow* window, int width, int height);
        static void window_content_scale_callback(GLFWwindow* window, float xscale, float yscale);
    };
} // Raven

#endif //RAVEN_CALLBACKS_H
