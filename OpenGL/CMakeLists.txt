cmake_minimum_required(VERSION 3.27)
project(OpenGL C CXX)

set(CMAKE_C_STANDARD 11)

include_directories(glad/include)

include_directories(glfw/include)

include_directories(include)

include_directories(glm/glm)

add_subdirectory(glad)

add_subdirectory(glfw)

add_subdirectory(glm)

add_library(utils STATIC
        src/shader.cpp
        src/texture.cpp
        src/init_window.cpp
        src/vertex_array.cpp
        src/camera.cpp
        include/camera.h)

add_executable(OpenGL src/main.cpp)

target_link_libraries(OpenGL utils)

target_link_libraries(OpenGL glad)

target_link_libraries(OpenGL glfw)

target_link_libraries(OpenGL glm)
