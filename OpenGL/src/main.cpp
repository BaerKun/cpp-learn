#include "texture.h"
#include "vertex_array.h"
#include "init_window.hpp"
#include "shader.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

int screenWidth = 800;
int screenHeight = 600;

float glfwTime;

float vertices1[] = {
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

unsigned int indices1[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
};

float vertices2[] = {
        .0f, 3.f, 0.f, 0.f,0.f,.0f,
        3.f, -1.f, 0.f, .0f,.0f,.0f,
        -3.f, -1.f, 0.f,.0f,.0f,.0f,
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main() {
    GLFWwindow *window = initWindow(screenWidth, screenHeight, "OpenGL");

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    ShaderProgram shaderProgram("../GLSL/vertshad1.vs",
                                "../GLSL/frgshad1.fs");
    ShaderProgram shaderProgram2("../GLSL/vertshad2.vs",
                                 "../GLSL/frgshad2.fs");

    Texture texture1("../imgs/awesomeface.png");
    Texture texture2("../imgs/container.jpg");

    shaderProgram.use();

    shaderProgram.setUniform("texture1", texture1.index);
    shaderProgram.setUniform("texture2", texture2.index);


    VertexArray vao1, vao2;
    VertexBuffer vbo1, vbo2;
    ElementBuffer ebo1;

    ebo1.use();
    ElementBuffer::setData(indices1, sizeof(indices1));

    vao1.use();
    vbo1.bind();
    VertexBuffer::setData(vertices1, sizeof(vertices1));
    VertexArray::setVertexAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), 0);
    VertexArray::setVertexAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
    VertexArray::setVertexAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float));

    vao2.use();
    vbo2.bind();
    VertexBuffer::setData(vertices2, sizeof(vertices2));
    VertexArray::setVertexAttribute(0, 3, GL_FLOAT, 6 * sizeof(float), 0);
    VertexArray::setVertexAttribute(1, 3, GL_FLOAT, 6 * sizeof(float), 3 * sizeof(float));

    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 unit = glm::mat4(1.0f);
    unit = glm::scale(unit, glm::vec3(0.5f, 0.5f, 0.5f));

    float r, g, b;
    float p;
    // 渲染循环
    while(!glfwWindowShouldClose(window)){
        glfwTime = glfwGetTime();

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        r = (glm::sin(glfwTime * 2.f + 1.f) + 1.0f) * .5f;
        g = (glm::sin(glfwTime * 2.f + 2.f) + 1.0f) * .5f;
        b = (glm::sin(glfwTime * 2.f + 3.f) + 1.0f) * .5f;

        vertices2[3] = r;
        vertices2[10] = g;
        vertices2[17] = b;

        vao2.use();
        vbo2.bind();
        VertexBuffer::setData(vertices2, sizeof(vertices2));

        shaderProgram2.use();
        VertexArray::drawArrays(GL_TRIANGLES, 3);

        p = (glm::sin(glfwTime) + .0f) * .1f;
        transform = glm::rotate(unit, glm::radians(glfwTime * 40.0f), glm::vec3(1.f, 1.f, 1.0f));
        transform = glm::translate(transform, glm::vec3(p, p, 0));

        vao1.use();
        ebo1.use();

        shaderProgram.use();
        shaderProgram.setUniformm("transform", glm::value_ptr(transform), 4);

        VertexArray::drawElements(GL_TRIANGLES, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放资源
    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // 设置视口大小
    glViewport(0, 0, screenWidth, screenHeight);
}

void processInput(GLFWwindow *window) {
    // ESC键退出
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}