#include<iostream>
#include<thread>
/*
 * C++语言级别的多线程编程=》代码可以跨平台
 *
 * 子线程如何结束
 * 子线程函数运行完成，线程就结束了
 *
 * 主线程如何处理子线程
 * join     等待子线程，当前线程继续往下运行
 * detach   分离子线程  当前进程结束所有线程结束
 */
void threadHandle1()
{
    //让子线程睡眠2秒
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"hello Thraed 1"<<std::endl;
}
int main()
{
    //定义了一个线程对象，传入一个线程函数，新线程开始运行
    std::thread t1(threadHandle1);
    //主线程等待子线程结束，主线程继续往下运行
    t1.join();
    //把子线程设置为分离线程
    t1.detach();
    std::cout<<"main Thraed done"<<std::endl;
    return 0;
}