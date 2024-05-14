#ifndef OPENGL_SHADER_HPP
#define OPENGL_SHADER_HPP

#include "glad/glad.h"
#include "glm.hpp"

extern GLuint CURRENT_SHADER_PROGRAM;

class ShaderProgram {
private:
    GLuint id;
public:
    ShaderProgram();

    ShaderProgram(GLuint , GLuint );

    ~ShaderProgram();

    void compile(const char *, const char *);

    void load(const char *vertex_path, const char *fragment_path);

    void use() const;

    void setUniform(const char *name, float value) const;

    void setUniform(const char *name, int value) const;

    void setUniformv(const char *name, const float *value, int count) const;

    void setUniformm(const char *name, const float *value, int rowcols) const;
};

#endif //OPENGL_SHADER_HPP
