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

add_subdirectory(src)

link_libraries(utils)

link_libraries(glad)
        
link_libraries(glfw)

link_libraries(glm)

add_executable(OpenGL src/main.cpp)

add_executable(FunctionDrawer src/function_drawer.cpp)


