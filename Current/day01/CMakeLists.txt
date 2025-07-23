cmake_minimum_required(VERSION 3.28)
project(taskqueue )

set(CMAKE_C_STANDARD 11)

add_executable(taskqueue test.cpp
        taskque.h
        taskque.cpp)
