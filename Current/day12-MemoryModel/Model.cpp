#include<iostream>
#include<atomic>
#include<thread>
#include<cassert>
#include<vector>
//全局一致性模型   memory_order_seq_cst
std::atomic<bool>x,y;
std::atomic<int>z;
void write_x_then_y()
{
    x.store(true,std::memory_order_seq_cst);//1
    y.store(true,std::memory_order_seq_cst);//2
}
void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst))
    {
        std::cout<<"y load false"<<std::endl;
    }
    if (x.load(std::memory_order_seq_cst))
    {
        ++z;
    }
}

void TestOrderSeqCst()
{
    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);
    t1.join();
    t2.join();
    assert(z.load()!=0);
}
//宽松内存序memory_order_relaxed
void TestOderRelaxed()
{
    std::atomic<bool>rx,ry;
    std::thread t1([&]()
    {
        rx.store(true,std::memory_order_relaxed);
        ry.store(true,std::memory_order_relaxed);
    });
    std::thread t2([&]()
    {
        while (!ry.load(std::memory_order_relaxed));
        assert(rx.load(std::memory_order_relaxed));
    });
    t1.join();
    t2.join();
}
//同步模型 Acquire_Release
void TestReleaseAcquire()
{
    std::atomic<bool>rx,ry;
    std::thread t1([&]()
    {
        rx.store(true,std::memory_order_relaxed);
        ry.store(true,std::memory_order_release);
    });
    std::thread t2([&]()
    {
        while (!ry.load(std::memory_order_acquire));
        assert(rx.load(std::memory_order_relaxed));
    });
    t1.join();
    t2.join();
}
//Release sequences
void ReleaseAcquireDanger()
{
    std::atomic<int>xd(0),yd(0);
    std::atomic<int>zd;

    std::thread t1([&]()
    {
        xd.store(true,std::memory_order_release);
        yd.store(true,std::memory_order_release);
    });
    std::thread t2([&]()
    {
        yd.store(2,std::memory_order_release);
    });
    std::thread t3([&]()
    {
        while (!yd.load(std::memory_order_acquire));
        assert(xd.load(std::memory_order_acquire)==1);
    });
    t1.join();
    t2.join();
    t3.join();
}
void ReleaseAcquireDanger2()
{
    std::atomic<int>xd{0},yd{0};
    std::atomic<int>zd;

    std::thread t1([&]()
    {
        xd.store(1,std::memory_order_release);
        yd.store(1,std::memory_order_release);
    });
    std::thread t2([&]()
    {
        yd.store(2,std::memory_order_release);
    });
    std::thread t3([&]()
    {
        while (!yd.load(std::memory_order_acquire));
        assert(xd.load(std::memory_order_acquire)==1);
    });
    t1.join();
    t2.join();
    t3.join();
}
//ReleaseSquence
void ReleaseSequence()
{
    std::vector<int>data;
    std::atomic<int>flag(0);
    std::thread t1([&]()
    {
        data.push_back(42);
        flag.store(1,std::memory_order_release);
    });
    std::thread t2([&]()
    {
        int expected=1;
        while (flag.compare_exchange_strong(expected,2,std::memory_order_relaxed))
            expected=1;
    });
    std::thread t3([&]()
    {
        while (flag.load(std::memory_order_acquire))
            assert(data.at(0)==42);
    });
    t1.join();
    t2.join();
    t3.join();
}

//memory_oder_consume
void ConsumeDependency()
{
    std::atomic<std::string*>ptr;
    int data;
    std::thread t1([&]()
    {
        std::string*p=new std::string("hello world");
        data=42;
        ptr.store(p,std::memory_order_release);
    });
    std::thread t2([&]()
    {
        std::string*p2;
        while (!(p2=ptr.load(std::memory_order_consume)));
        assert(data==42);
    });
    t1.join();
    t2.join();
}
int main()
{
    //TestOrderSeqCst();
    //TestOderRelaxed();
    TestReleaseAcquire();
    return 0;
}