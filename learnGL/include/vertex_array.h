#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "glad/glad.h"
#include <iostream>

extern GLuint CURRENT_VAO, CURRENT_VBO, CURRENT_EBO;

class VertexArray {
public:
    GLuint id;

    VertexArray();

    void use() const;

    void unuse() const;

    static void setVertexAttribute(int location, int count, GLenum type, int stride, int offset);

    static void drawArrays(GLenum mode, int count);

    static void drawElements(GLenum mode, int count);

    ~VertexArray();
};

class VertexBuffer{
public:
    GLuint id;

    VertexBuffer();

    static void setData(void *data, int size);

    void bind() const;

    void unbind() const;

   ~VertexBuffer();
};

class ElementBuffer{
public:
    GLuint id;

    ElementBuffer();

    static void setData(void *data, int size);

    void use() const;

    void unuse() const;

    ~ElementBuffer();
};

#endif //OPENGL_VERTEX_ARRAY_H
