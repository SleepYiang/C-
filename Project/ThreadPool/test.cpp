#include<iostream>
#include <thread>

#include"ThreadPool.h"
int main()
{
    ThreadPool pool;
    pool.start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}