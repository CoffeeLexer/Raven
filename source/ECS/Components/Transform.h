//
// Created by tartarus on 11/12/22.
//

#ifndef RAVEN_TRANSFORM_H
#define RAVEN_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Raven {

    struct Transform
    {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
    }

} // Raven

#endif //RAVEN_TRANSFORM_H
