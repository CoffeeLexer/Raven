//
// Created by tartarus on 11/29/22.
//

#ifndef RAVEN_TYPEID_H
#define RAVEN_TYPEID_H

#include <cstdio>

namespace Raven {
    class TypeID {

        static size_t counter;

    public:
        template<typename T>
        static size_t value() {
            static size_t value = counter++;
            return value;
        }
    };
}

#endif //RAVEN_TYPEID_H
