//
// Created by tartarus on 11/25/22.
//

#ifndef RAVEN_CALLBACK_H
#define RAVEN_CALLBACK_H

#include <vector>
#include <functional>

namespace Raven {
    template<typename T>
    class Callback {
    public:

        std::vector<std::function<std::remove_pointer_t<T>>> functions;

        template<typename ...Args>
        void invoke(Args &&... args) {
            for(const auto& f: functions) {
                f(args...);
            }
        }

        void append(std::function<std::remove_pointer_t<T>> function) {
            functions.push_back(function);
        }

    };

} // Raven

#endif //RAVEN_CALLBACK_H
