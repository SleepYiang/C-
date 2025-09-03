cmake_minimum_required(VERSION 3.28)
project(radix_sort)

set(CMAKE_CXX_STANDARD 20)
find_package(fmt REQUIRED)

add_executable(radix_sort main.cpp)
# 链接 fmt（与之前一致）
target_link_libraries(radix_sort PRIVATE fmt::fmt)