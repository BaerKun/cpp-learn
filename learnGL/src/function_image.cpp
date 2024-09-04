#include "function_image.hpp"
#include "camera.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>


#define EXPRESSION_POSITION 134
#define EXPRESSION_MAX_LENGTH 64

char vertexTemplate[] = "#version 330 core\n"
                      "layout (location = 0) in vec3 aPos;"
                      "uniform mat4 transform;"
                      "void main(){"
                      "float x = aPos[1];"
                      "float y = aPos[0];"
                      "float z ="
                      "                                            ;"
                      "gl_Position = transform * vec4(y, z, x, 1.0);"
                      "}";

char fragmentSource[] = "#version 330 core\n"
                        "out vec4 FragColor;"
                        "uniform vec3 color;"
                        "void main(){"
                        "FragColor = vec4(color, 1.0f);"
                        "}";

ShaderProgram axesShaderProgram;
VertexArray *axesPoints;
VertexBuffer *axesBuffer;

static float randZero2One() {
    static unsigned int i = 0;
    srand((unsigned int) time(nullptr) + i++);
    return (float) (rand() % 256) / 256.f;
}

static float *range(float left, float right, unsigned count) {
    if (count == 0)
        return nullptr;

    float *points = new float[2 * count];

    if (count == 1) {
        *points = (left + right) / 2.f;
        points[1] = 0.f;
    } else {
        float delta = (right - left) / (float) (count - 1);

        for (unsigned i = 0; i < count; ++i) {
            points[i << 1] = left;
            points[1 + (i << 1)] = 0.f;
            left += delta;
        }
    }
    return points;
}

static void loadExpression(const char *expression, unsigned length) {
    char *ptrExpression = vertexTemplate + EXPRESSION_POSITION;
    memcpy(ptrExpression, expression, length);
}

void init(){
    axesShaderProgram.compile(
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;"
            "uniform mat4 transform;"
            "void main(){"
            "   gl_Position = transform * vec4(aPos, 1.f);"
            "}",
            "#version 330 core\n"
            "out vec4 FragColor;"
            "void main(){"
            "   FragColor = vec4(0.f, 0.f, 0.f, 1.f);"
            "}"
            );

    axesPoints = new VertexArray;
    axesBuffer = new VertexBuffer;

    axesPoints->use();
    axesBuffer->bind();

    float points[] = {100.f, 0.f, 0.f,
                      -100.f, 0.f, 0.f,
                      0.f, 100.f, 0.f,
                      0.f, -100.f, 0.f,
                      0.f, 0.f, 100.f,
                      0.f, 0.f, -100.f
    };

    VertexBuffer::setData(points, sizeof(points));
    VertexArray::setVertexAttribute(0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    axesPoints->unuse();
    axesBuffer->unbind();
}

void drawAxes(){
    axesPoints->use();
    axesBuffer->bind();
    axesShaderProgram.use();

    axesShaderProgram.setUniformm("transform", glm::value_ptr(currentCamera->getMatrix()), 4);
    VertexArray::drawArrays(GL_LINES, 6);
}

Function::Function(const char *str_expression) {
    unsigned length = strlen(str_expression);
    if (length > EXPRESSION_MAX_LENGTH) {
        printf("too long");
        return;
    }

    loadExpression(str_expression, length);

    float color[3];
    color[0] = randZero2One();
    color[1] = randZero2One();
    color[2] = randZero2One();

    this->shaderProgram.compile(vertexTemplate, fragmentSource);
    shaderProgram.use();
    this->shaderProgram.setUniformv("color", color, 3);

    float *pointsPos = range(-10.f, 10.f, 1000);

    this->points.use();
    pointsBuffer.bind();

    VertexBuffer::setData(pointsPos, 2000 * sizeof(float));
    VertexArray::setVertexAttribute(0, 2, GL_FLOAT, 2 * sizeof(float), 0);

    this->points.unuse();
    pointsBuffer.unbind();

    delete[] pointsPos;
}

void Function::draw() {
    points.use();
    pointsBuffer.bind();
    shaderProgram.use();

    glm::mat4 transform = currentCamera->getMatrix();
    shaderProgram.setUniformm("transform", glm::value_ptr(transform), 4);

    VertexArray::drawArrays(GL_LINE_STRIP, 1000);

    points.unuse();
    pointsBuffer.unbind();
    shaderProgram.use();
}