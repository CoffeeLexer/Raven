//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_SYSTEM_H
#define RAVEN_SYSTEM_H

#include <set>

#include "Entity.h"

namespace Raven {
    class System
    {
    public:
        std::set<Entity> mEntities;
    };
}

#endif //RAVEN_SYSTEM_H
