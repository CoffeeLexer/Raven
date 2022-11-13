//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_ENTITY_H
#define RAVEN_ENTITY_H

#include <cstdint>

namespace Raven {
    using Entity = std::uint32_t;

    const Entity ENTITY_MAX = 1 << 12;


} // Raven

#endif //RAVEN_ENTITY_H
