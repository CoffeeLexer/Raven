//
// Created by tartarus on 11/6/22.
//

#include <string>
#include "FileSystem.h"

namespace Raven {
    std::string FileSystem::getExtension(const std::string &file) {
        size_t index = file.find_last_of('.');
        size_t length = file.length() - index;

        char* extension = new char[length];
        extension[length - 1] = '\0';

        file.copy(extension, length - 1, index + 1);
        std::string result = std::string(extension);

        delete[] extension;

        return result;
    }
} // Raven