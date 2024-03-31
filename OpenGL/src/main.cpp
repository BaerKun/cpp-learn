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
#include "camera.h"

int screenWidth = 800;
int screenHeight = 600;

float lastTime;
float currentTime;
float deltaTime;

Camera camera;

float vertices1[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main() {
    GLFWwindow *window = initWindow(screenWidth, screenHeight, "OpenGL");

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    ShaderProgram shaderProgram("../GLSL/vertshad1.vs",
                                "../GLSL/frgshad1.fs");

    stbi_set_flip_vertically_on_load(true);
    Texture texture1("../imgs/awesomeface.png");
    Texture texture2("../imgs/container.jpg");

    shaderProgram.use();

    shaderProgram.setUniform("texture1", texture1.index);
    shaderProgram.setUniform("texture2", texture2.index);

    VertexArray vao1;
    VertexBuffer vbo1;

    vao1.use();
    vbo1.bind();
    VertexBuffer::setData(vertices1, sizeof(vertices1));
    VertexArray::setVertexAttribute(0, 3, GL_FLOAT, 5 * sizeof(float), 0);
    VertexArray::setVertexAttribute(1, 2, GL_FLOAT, 5 * sizeof(float), 3 * sizeof(float));



    glm::mat4 transform;
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

//    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

    texture1.use();
    texture2.use();
    float r, g, b;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // 渲染循环
    while(!glfwWindowShouldClose(window)){
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;

        processInput(window);

        r = (glm::sin(currentTime * 2.f + 1.f) + 1.0f) * .5f;
        g = (glm::sin(currentTime * 2.f + 2.f) + 1.0f) * .5f;
        b = (glm::sin(currentTime * 2.f + 3.f) + 1.0f) * .5f;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(glm::mat4(1.f), glm::radians(50.f * currentTime), glm::vec3(0.5f, 1.0f, 0.0f));
        view = camera.viewMatrix;

        transform = projection * view * model;

        vao1.use();

        shaderProgram.use();

        shaderProgram.setUniformm("transform", glm::value_ptr(transform), 4);

        VertexArray::drawArrays(GL_TRIANGLES, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;
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
    float speed = 2.5f;
    // ESC键退出
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.move(0, speed * deltaTime, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.move(0, -speed * deltaTime, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.move(-speed * deltaTime, 0, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.move(speed * deltaTime, 0, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        camera.move(0, 0, speed * deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        camera.move(0, 0, -speed * deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        camera.rotate(0, 20.f * speed * deltaTime, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        camera.rotate(0, -20.f * speed * deltaTime, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        camera.rotate(20.f * speed * deltaTime, 0, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        camera.rotate(-20.f * speed * deltaTime, 0, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera.rotate(0, 0, 20.f * speed * deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera.rotate(0, 0, -20.f * speed * deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        camera.lookAt(glm::vec3(0, 0, 0));
    }
}