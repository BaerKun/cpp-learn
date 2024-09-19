cmake_minimum_required(VERSION 3.27)

project(learnCV)

if(WIN32)
    set(OpenCV_DIR "C:\\_myLibs\\opencv-4.10\\build")
endif()

find_package(OpenCV)

include_directories(${OpenCV_DIRS})

link_libraries(${OpenCV_LIBS})

add_executable(main main.cpp)

add_library(somealg myalg.cpp)

target_link_libraries(main somealg)