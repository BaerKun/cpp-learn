#include "shader.hpp"
#include <iostream>
#include <cstdio>

GLuint createShader(const char * &path, GLenum type, char * &source, char * &infoLog){
    FILE *file = fopen(path, "r");

    if(file == nullptr){
       std::cerr << "Failed to open shader source file: " << path << std::endl;
        return 0;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    fread(source, 1, size, file);
    source[size] = '\0';

    fclose(file);

    int success;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 128, nullptr, infoLog);
        std::cerr << "Failed to compile shader: " << infoLog << std::endl;
        return 0;
    }

    return shader;
}

ShaderProgram::ShaderProgram(const char *vertex_path, const char *fragment_path) {
    char *source = new char[256];
    char *infoLog = new char[128];

    GLuint vertexShader = createShader(vertex_path, GL_VERTEX_SHADER, source, infoLog);
    GLuint fragmentShader = createShader(fragment_path, GL_FRAGMENT_SHADER, source, infoLog);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, 128, &success, infoLog);
        std::cerr << "Failed to link shader program: " << infoLog << std::endl;
    }

    this->id = program;

    delete[] source;
    delete[] infoLog;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use() const {
    glUseProgram(this->id);
}

void ShaderProgram::setUniform(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(this->id, name), value);
}

void ShaderProgram::setUniform(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(this->id, name), value);
}

void ShaderProgram::setUniformv(const char *name, float *value, int count) const {
    GLuint location = glGetUniformLocation(this->id, name);
    switch (count) {
        case 4:
            glUniform4fv(location, 1, value);
            break;
        case 3:
            glUniform3fv(location, 1, value);
            break;
        case 2:
            glUniform2fv(location, 1, value);
        default:
            break;
    }
}

void ShaderProgram::setUniformm(const char *name, float *value, int rowcols) const {
    GLuint location = glGetUniformLocation(this->id, name);
    switch (rowcols) {
        case 4:
            glUniformMatrix4fv(location, 1, GL_FALSE, value);
            break;
        case 3:
            glUniformMatrix3fv(location, 1, GL_FALSE, value);
            break;
        case 2:
            glUniformMatrix2fv(location, 1, GL_FALSE, value);
        default:
            break;
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(this->id);
}