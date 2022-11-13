//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_COMPONENT_H
#define RAVEN_COMPONENT_H

#include <cstdint>
#include <bitset>

namespace Raven {

    using Component = std::uint8_t;
    const Component COMPONENT_MAX = 32;

    // TODO: Signature size is determined by using CMAKE FILE GLOP
    using Signature = std::bitset<COMPONENT_MAX>;

} // Raven

#endif //RAVEN_COMPONENT_H
