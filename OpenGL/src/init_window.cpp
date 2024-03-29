#include "init_window.hpp"
#include <iostream>

GLFWwindow *initWindow(int width, int height, const char *title){
    // 初始化GLFW
    glfwInit();

    // 设置OpenGL版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // 设置OpenGL为核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if(window == nullptr){
        puts("Failed to create GLFW window");
        return nullptr;
    }
    // 设置窗口上下文
    glfwMakeContextCurrent(window);

    // 初始化GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        puts("Failed to initialize GLAD");
    }

    return window;
}