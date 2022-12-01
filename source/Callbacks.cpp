//
// Created by tartarus on 11/24/22.
//

#include "Callbacks.h"
#include <typeinfo>
#include <functional>

namespace Raven {
    Callback<GLFWerrorfun> Callbacks::errorCallbacks = {};
    Callback<GLFWjoystickfun> Callbacks::joystickCallbacks = {};
    Callback<GLFWmonitorfun> Callbacks::monitorCallbacks = {};

    Callbacks *Callbacks::ptr = nullptr;

    static bool isGlobalsBound = false;
    static void bindGlobals() {
        glfwSetErrorCallback(Callbacks::error_callback);
        glfwSetJoystickCallback(Callbacks::joystick_callback);
        glfwSetMonitorCallback(Callbacks::monitor_callback);
        isGlobalsBound = true;
    }

    Callbacks::Callbacks() {
        mDictionary = {};
    }

    template<typename T>
    void Callbacks::append(Types key, std::function<std::remove_pointer_t<T>> callback) {
        auto got = mDictionary.find(key);
        if(got == mDictionary.end()) {
            auto *ptr = new Callback<T>();
            ptr->append(callback);
            mDictionary.emplace(key, ptr);
        }
        else {
            reinterpret_cast<Callback<T>*>(got->second)->append(callback);
        }
    }

    template<typename T>
    void Callbacks::appendGlobal(Callbacks::TypesGlobal key, std::function<std::remove_pointer_t<T>> callback) {
        if(!isGlobalsBound) bindGlobals();
        switch (key) {
            case TypesGlobal::Error:
                errorCallbacks.append(reinterpret_cast<const std::function<void(int, const char *)> &>(callback));
                break;
            case TypesGlobal::Joystick:
                joystickCallbacks.append(reinterpret_cast<const std::function<void(int, int)> &>(callback));
                break;
            case TypesGlobal::Monitor:
                monitorCallbacks.append(reinterpret_cast<const std::function<void(GLFWmonitor *, int)> &>(callback));
                break;
        }
    }

    void Callbacks::bind(GLFWwindow *window) {
        //  Bind window's user pointer to our callbacks object, for invoke<> method purposes
        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCharCallback(window, char_callback);
        glfwSetDropCallback(window, drop_callback);
        //  Error callback is global
        //  Joystick callback is global
        //  Monitor callback is global
        glfwSetCursorEnterCallback(window, cursor_enter_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCharModsCallback(window, char_mods_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowCloseCallback(window, window_close_callback);
        glfwSetWindowFocusCallback(window, window_focus_callback);
        glfwSetWindowIconifyCallback(window, window_iconify_callback);
        glfwSetWindowMaximizeCallback(window, window_maximize_callback);
        glfwSetWindowPosCallback(window, window_position_callback);
        glfwSetWindowRefreshCallback(window, window_refresh_callback);
        glfwSetWindowSizeCallback(window, window_size_callback);
        glfwSetWindowContentScaleCallback(window, window_content_scale_callback);
    }

    template<typename T, typename ...Args>
    void Callbacks::invoke(Types key, Args &&...args) {
        auto got = mDictionary.find(key);
        if(got != mDictionary.end()) {
            reinterpret_cast<Callback<T>*>(got->second)->invoke(args...);
        }
    }

//    template<Callbacks::TypesGlobal T, typename ...Args>
//    void Callbacks::invokeGlobal(Args &&...args) {}


    template<>
    void Callbacks::invokeGlobal<Callbacks::TypesGlobal::Error>(int &error_code, const char*& description) {
        errorCallbacks.invoke(error_code, description);
    }

    template<>
    void Callbacks::invokeGlobal<Callbacks::TypesGlobal::Joystick>(int &jid, int &event) {
        joystickCallbacks.invoke(jid, event);
    }

    template<>
    void Callbacks::invokeGlobal<Callbacks::TypesGlobal::Monitor>(GLFWmonitor*& monitor, int &event) {
        monitorCallbacks.invoke(monitor, event);
    }

    void Callbacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWscrollfun>(Types::Scroll, window, xoffset, yoffset);
    }
    void Callbacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWkeyfun>(Types::Key, window, key, scancode, action, mods);
    }
    void Callbacks::char_callback(GLFWwindow* window, unsigned int codepoint) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWcharfun>(Types::Char, window, codepoint);
    }
    void Callbacks::drop_callback(GLFWwindow* window, int path_count, const char* paths[]) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWdropfun>(Types::Char, window, path_count, paths);
    }
    void Callbacks::error_callback(int error_code, const char* description) {
        Callbacks::invokeGlobal<TypesGlobal::Error>(error_code, description);
    }
    void Callbacks::joystick_callback(int jid, int event) {
        Callbacks::invokeGlobal<TypesGlobal::Joystick>(jid, event);
    }
    void Callbacks::monitor_callback(GLFWmonitor* monitor, int event) {
        Callbacks::invokeGlobal<TypesGlobal::Monitor>(monitor, event);
    }
    void Callbacks::cursor_enter_callback(GLFWwindow* window, int entered) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWcursorenterfun>(Types::CursorEnter, window, entered);
    }
    void Callbacks::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWcursorposfun>(Types::CursorPosition, window, xpos, ypos);
    }
    void Callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWmousebuttonfun>(Types::MouseButton, window, button, action, mods);
    }
    void Callbacks::char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWcharmodsfun>(Types::CharMods, window, codepoint, mods);
    }
    void Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWframebuffersizefun>(Types::FramebufferSize, window, width, height);
    }
    void Callbacks::window_close_callback(GLFWwindow* window) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowclosefun>(Types::WindowClose, window);
    }
    void Callbacks::window_focus_callback(GLFWwindow* window, int focused) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowfocusfun>(Types::WindowFocus, window, focused);
    }
    void Callbacks::window_iconify_callback(GLFWwindow* window, int iconified) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowiconifyfun>(Types::WindowIconify, window, iconified);
    }
    void Callbacks::window_maximize_callback(GLFWwindow* window, int maximized) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowmaximizefun>(Types::WindowMaximize, window, maximized);
    }
    void Callbacks::window_position_callback(GLFWwindow* window, int xpos, int ypos) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowposfun>(Types::WindowPosition, window, xpos, ypos);
    }
    void Callbacks::window_refresh_callback(GLFWwindow* window) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowrefreshfun>(Types::WindowRefresh, window);
    }
    void Callbacks::window_size_callback(GLFWwindow* window, int width, int height) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowsizefun>(Types::WindowSize, window, width, height);
    }
    void Callbacks::window_content_scale_callback(GLFWwindow* window, float xscale, float yscale) {
        auto *callbacks = reinterpret_cast<Callbacks *>(glfwGetWindowUserPointer(window));
        callbacks->invoke<GLFWwindowcontentscalefun>(Types::WindowContentScale, window, xscale, yscale);
    }

    template void Callbacks::appendGlobal<GLFWerrorfun>(TypesGlobal key, std::function<std::remove_pointer_t<GLFWerrorfun>> callback);
    template void Callbacks::appendGlobal<GLFWjoystickfun>(TypesGlobal key, std::function<std::remove_pointer_t<GLFWjoystickfun>> callback);
    template void Callbacks::appendGlobal<GLFWmonitorfun>(TypesGlobal key, std::function<std::remove_pointer_t<GLFWmonitorfun>> callback);

//  Commented templates are type duplicates
    template void Callbacks::append<GLFWscrollfun>(Types key, std::function<std::remove_pointer_t<GLFWscrollfun>> callback);
    template void Callbacks::append<GLFWkeyfun>(Types key, std::function<std::remove_pointer_t<GLFWkeyfun>> callback);
    template void Callbacks::append<GLFWcharfun>(Types key, std::function<std::remove_pointer_t<GLFWcharfun>> callback);
    template void Callbacks::append<GLFWdropfun>(Types key, std::function<std::remove_pointer_t<GLFWdropfun>> callback);
    template void Callbacks::append<GLFWcursorenterfun>(Types key, std::function<std::remove_pointer_t<GLFWcursorenterfun>> callback);
//    template void Callbacks::append<GLFWcursorposfun>(size_t key, std::function<std::remove_pointer_t<GLFWcursorposfun>> callback);
    template void Callbacks::append<GLFWmousebuttonfun>(Types key, std::function<std::remove_pointer_t<GLFWmousebuttonfun>> callback);
    template void Callbacks::append<GLFWcharmodsfun>(Types key, std::function<std::remove_pointer_t<GLFWcharmodsfun>> callback);
    template void Callbacks::append<GLFWframebuffersizefun>(Types key, std::function<std::remove_pointer_t<GLFWframebuffersizefun>> callback);
    template void Callbacks::append<GLFWwindowclosefun>(Types key, std::function<std::remove_pointer_t<GLFWwindowclosefun>> callback);
//    template void Callbacks::append<GLFWwindowfocusfun>(size_t key, std::function<std::remove_pointer_t<GLFWwindowfocusfun>> callback);
//    template void Callbacks::append<GLFWwindowiconifyfun>(size_t key, std::function<std::remove_pointer_t<GLFWwindowiconifyfun>> callback);
//    template void Callbacks::append<GLFWwindowmaximizefun>(size_t key, std::function<std::remove_pointer_t<GLFWwindowmaximizefun>> callback);
//    template void Callbacks::append<GLFWwindowposfun>(size_t key, std::function<std::remove_pointer_t<GLFWwindowposfun>> callback);
//    template void Callbacks::append<GLFWwindowrefreshfun>(size_t key, std::function<std::remove_pointer_t<GLFWwindowrefreshfun>> callback);
//    template void Callbacks::append<GLFWwindowrefreshfun>(size_t key, std::function<std::remove_pointer_t<GLFWwindowrefreshfun>> callback);
    template void Callbacks::append<GLFWwindowcontentscalefun>(Types key, std::function<std::remove_pointer_t<GLFWwindowcontentscalefun>> callback);

    //  These callbacks are global callbacks - DOES NOT LINK TO WINDOW INSTANCE
//    template void Callbacks::append<GLFWerrorfun>(Types key, std::function<std::remove_pointer_t<GLFWerrorfun>> callback);
    template void Callbacks::append<GLFWjoystickfun>(Types key, std::function<std::remove_pointer_t<GLFWjoystickfun>> callback);
    template void Callbacks::append<GLFWmonitorfun>(Types key, std::function<std::remove_pointer_t<GLFWmonitorfun>> callback);
} // Raven