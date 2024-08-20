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
#include "function_image.hpp"

int screenWidth = 800;
int screenHeight = 600;

float lastTime;
float currentTime;
float deltaTime;

Camera camera;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main() {
    GLFWwindow *window = initWindow(screenWidth, screenHeight, "OpenGL");

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glm::mat4 transform;
    glm::mat4 model         = glm::mat4(1.0f);

    camera.use();
    camera.setProjection((float)screenWidth / (float)screenHeight, 10.f);
    camera.move(0, 0, 5.f);

    char strFunction[64];
    scanf("%s", strFunction);

    Function fun(strFunction);
    init();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // 渲染循环
    while(!glfwWindowShouldClose(window)){
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;

        processInput(window);

        glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawAxes();
        fun.draw();

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