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

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::use() {
        glBindTexture(GL_TEXTURE_2D, _id);
    }
} // Raven