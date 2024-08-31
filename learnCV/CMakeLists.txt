cmake_minimum_required(VERSION 3.27)

project(learnCV)

find_package(OpenCV)

include_directories(${OpenCV_DIRS})

link_libraries(${OpenCV_LIBS})

add_executable(main main.cpp)

add_library(somealg myalg.cpp)