#ifndef FUNCTION_IMAGE_HPP
#define FUNCTION_IMAGE_HPP

#include "shader.hpp"
#include "vertex_array.h"

class Function{
public:
    Function(const char *str_expression);

    void draw();
private:
    ShaderProgram shaderProgram;
    VertexArray points;
    VertexBuffer pointsBuffer;
};

void init();

void drawAxes();

#endif // FUNCTION_IMAGE_HPP