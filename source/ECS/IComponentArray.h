//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_I_COMPONENT_ARRAY_H
#define RAVEN_I_COMPONENT_ARRAY_H

#include "Entity.h"

namespace Raven {
    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
    };
}

#endif //RAVEN_I_COMPONENT_ARRAY_H
