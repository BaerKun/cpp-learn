#ifndef OPENGL_SHADER_HPP
#define OPENGL_SHADER_HPP

#include "glad/glad.h"
#include "glm.hpp"

class ShaderProgram {
public:
    GLuint id;

    ShaderProgram(const char *vertex_path, const char *fragment_path);

    ~ShaderProgram();

    void use() const;

    void setUniform(const char *name, float value) const;

    void setUniform(const char *name, int value) const;

    void setUniformv(const char *name, float *value, int count) const;

    void setUniformm(const char *name, float *value, int rowcols) const;
};

#endif //OPENGL_SHADER_HPP
