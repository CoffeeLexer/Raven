//
// Created by tartarus on 11/10/22.
//

#ifndef RAVEN_TEXTURE_H
#define RAVEN_TEXTURE_H

#include <string>

namespace Raven {

    class Texture {
        unsigned int _id;
    public:
        Texture();
        Texture(std::__cxx11::basic_string<char> path);
        void use();
    };

} // Raven

#endif //RAVEN_TEXTURE_H
