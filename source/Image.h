//
// Created by tartarus on 11/10/22.
//

#ifndef RAVEN_IMAGE_H
#define RAVEN_IMAGE_H

#include <string>

namespace Raven {

    class Image {
        unsigned char* _data;
        int _width;
        int _height;
        int _channel;
        int _currentChannel;

    public:
        Image(std::string path, int forceComponentNumber = 0);
        int getWidth() const {return _width;}
        int getHeight() const {return _height;}
        int getNativeChannel() const {return _channel;}
        int getCurrentChannel() const {return _currentChannel;}
        unsigned char* data() const {return _data;}
    };

} // Raven

#endif //RAVEN_IMAGE_H
