//
// Created by tartarus on 11/10/22.
//
#include <stb_image.h>
#include <glad/glad.h>
#include "Texture.h"
#include "Image.h"

namespace Raven {
    Texture::Texture(std::string path) {
        stbi_set_flip_vertically_on_load(true);
        Raven::Image img(path);

        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        switch (img.getNativeChannel()) {
            case 1:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, img.data());
            case 2:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RG, GL_UNSIGNED_BYTE, img.data());
            case 3:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.data());
            case 4:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.data());
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::use() const {
        glBindTexture(GL_TEXTURE_2D, _id);
    }
} // Raven