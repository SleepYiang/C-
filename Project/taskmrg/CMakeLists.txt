cmake_minimum_required(VERSION 3.31)
project(taskmrg)

set(CMAKE_CXX_STANDARD 17)

add_executable(taskmrg main.cpp Logger.h Logger.cpp
        Task.h
        TaskManager.cpp
        TaskManager.h
        Command.cpp
        Command.h)