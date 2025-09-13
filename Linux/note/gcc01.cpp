//Cameke相关的笔记
/*、
#设置编译的CMake版本
cmake_minimum_required(VERSION 3.28.3)
#设置项目名称
project(Linux)
#设置编译选项
set(CMAKE_CXX_FLGS ${CMAKE_CXX_FLAGS}-g)
#配置头文件
#include_directories()
#配置库文件搜索路径
#link_directories()
#设置c++的标准
set(CMAKE_CXX_STANDARD 11)
#set(muduo_DIR "/usr/include")
# 查找fmt库            /usr/include
#find_package(REQUIRED muduo)
#配置编译的文件
add_executable(Linux main.cpp
        muduo/TcpClient.cpp)
#把.指定路径下的所有源文件名字放入SRC_LIST里面
#aux_source_directory(.SRC_LIST)
# 链接fmt库
target_link_libraries(Linux muduo_net muduo_base pthread)
 */












//什么是gcc       gun编译套件  编译器可以编译c++语言
//什么是库    1.代码保密     2.方便部署和开发


//lib 前缀     xxxx 库的名字     .a后缀
//静态库的制作
    //gcc 获得.o文件
    //将.o文件打包，使用ar工具
//ar res libxxxx.a xxx.o xxx.o
//r-将文件插入备存文件中
//c-建立备存文件
//s-索引

//动态库的制作
//lib 前缀     xxxx 库的名字     .so后缀 (在linux是一个可执行文件)
//动态库的制作（共享库）
    //gcc得到.o文件 得到和位置无关的代码        gcc -c -fpic/-fPIC
    //得到动态库    gcc -shared a.o b.o -o libcalc.so
//解决动态库加载失败的问题（没有提供加载的绝对路径）
    //静态库 链接时，会把静态库中代码打包到可执行程序中
    //动态库 链接时，动态库的代码不会被打包到可执行程序中

