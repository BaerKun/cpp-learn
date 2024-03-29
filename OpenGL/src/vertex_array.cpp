#include "vertex_array.h"

GLuint CURRENT_VAO, CURRENT_VBO, CURRENT_EBO;

VertexArray::VertexArray(){
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &id);
}

void VertexArray::use() const{
    glBindVertexArray(id);
    CURRENT_VAO = id;
}

void VertexArray::unuse() const {
    if(CURRENT_VAO == id) {
        glBindVertexArray(0);
        CURRENT_VAO = id;
    }
}

void VertexArray::setVertexAttribute(int index, int count, GLenum type, int stride, int offset) {
    glVertexAttribPointer(index, count, type, GL_FALSE, stride, (void *)offset);
    glEnableVertexAttribArray(index);
}

void VertexArray::drawArrays(GLenum mode, int count) {
    glDrawArrays(mode, 0, count);
}

void VertexArray::drawElements(GLenum mode, int count) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
}

VertexBuffer::VertexBuffer(){
    glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &id);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    CURRENT_VBO = id;
}

void VertexBuffer::unbind() const {
    if(CURRENT_VBO == id) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CURRENT_VBO = 0;
    }
}

void VertexBuffer::setData(void *data, int size){
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ElementBuffer::ElementBuffer(){
    glGenBuffers(1, &id);
}

ElementBuffer::~ElementBuffer(){
    glDeleteBuffers(1, &id);
}

void ElementBuffer::use() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    CURRENT_EBO = id;
}

void ElementBuffer::unuse() const {
    if(CURRENT_EBO == id) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        CURRENT_EBO = 0;
    }
}

void ElementBuffer::setData(void *indices, int size){
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}