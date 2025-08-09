#include<iostream>
#include<atomic>
#include<thread>
#include<cassert>
std::atomic<bool>x,y;
std::atomic<int>z;
void write_x()
{
    x.store(true,std::memory_order_release);
}
void write_y()
{
    y.store(true,std::memory_order_release);
}
void read_x_then_y()
{
    while (!x.load(std::memory_order_acquire));
    if (y.load(std::memory_order_acquire))
        ++z;
}
void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire));
    if (x.load(std::memory_order_acquire))
            ++z;
}
void TestAR()
{
    x=false;
    y=false;
    z=0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load()!=0);
    std::cout<<"z value is"<<z.load()<<std::endl;
}
//栅栏实现同步
void write_x_then_y_fence()
{
    x.store(true,std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);
    y.store(true,std::memory_order_relaxed);
}
void read_y_then_x_fence()
{
    while (!y.load(std::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order_acquire);
    if (x.load(std::memory_order_relaxed))
        ++z;
}
int main()
{
    TestAR();
    return 0;
}