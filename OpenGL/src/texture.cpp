#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture(const char *path){
    glGenTextures(1, &id);
    index = id - 1;

    glBindTexture(GL_TEXTURE_2D, id);

    unsigned char *data;
    data = stbi_load(path, &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height,0,
                     (channels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::setParam(GLenum param, GLenum value) {
    glTexParameteri(GL_TEXTURE_2D, param, value);
}

void Texture::use() const{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, id);
}