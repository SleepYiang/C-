cmake_minimum_required(VERSION 3.28.3)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 20)

add_executable(ThreadPool ThreadPool.cpp ThreadPool.h test.cpp)
