#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "glad/glad.h"

class Texture {
public:
    GLuint id;
    int index;
    int width, height, channels;

    Texture(const char *path);

    ~Texture();

    void bind() const;

    static void setParam(GLenum param, GLenum value);

    void use() const;
};

#endif //OPENGL_TEXTURE_H
