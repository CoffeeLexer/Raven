//
// Created by tartarus on 11/10/22.
//
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <exception>
#include <stdexcept>
#include <cassert>

#include "Image.h"


namespace Raven {
    Image::Image(std::string path, int forceComponentNumber) {
        _data = stbi_load(path.c_str(), &_width, &_height, &_channel, forceComponentNumber);
#ifdef DEBUG
        if(_data == NULL) {
            std::string error = "IMAGE::CONSTRUCTOR::FILE\n\t";
            error += path;
            error += "\n\t";
            error += stbi_failure_reason();
            throw std::runtime_error(error);
        }
#endif
        _currentChannel = forceComponentNumber ? forceComponentNumber : _channel;
    }
} // Raven