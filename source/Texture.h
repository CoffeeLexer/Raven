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
        Texture(std::string path);
        void use() const;
    };

} // Raven

#endif //RAVEN_TEXTURE_H
