#设置编译的CMake版本
cmake_minimum_required(VERSION 3.28.3)
#设置项目名称
project(Linux)
#设置c++的标准
set(CMAKE_CXX_STANDARD 11)
#set(muduo_DIR "/usr/include")
# 查找fmt库            /usr/include
#find_package(REQUIRED muduo)
#配置编译的文件
add_executable(Linux network/selectServer.cpp)
# 链接fmt库
target_link_libraries(Linux muduo_net muduo_base pthread mysqlclient)
