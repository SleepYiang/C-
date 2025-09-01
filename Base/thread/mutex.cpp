#include<iostream>
#include <list> //C++所有容器都不是线程安全
#include<thread>
#include<mutex>
using namespace std;
/*
 * c++ thread 模拟车站100张车票，由三个窗口一起买票
 */
int Ticketcount=100;
std::mutex mtx;
//卖票的线程函数

//多线程程序
//竞态条件：多线程程序执行的结果是一致的，不会随着cpu对象不同的顺序，而产生不同的运行结果。
void sellTicket(int index)
{

    while(Ticketcount>0)
    {
        mtx.lock();
        //临界区代码段-》原子操作-》线程间互斥操作-》mutex
        std::cout<<"窗口"<<index<<"卖出第："<<Ticketcount<<"张票"<<endl;
        Ticketcount--;
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(100));

    }

}
int main()
{
    //lock_gurad
    //unique_lock
    pmr::list<std::thread>tlist;
    for (int i=0;i<3;i++)
    {
        tlist.push_back(std::thread(sellTicket,i));
    }
    for (std::thread& t:tlist)
    {
        t.join();
    }
    std::cout<<"所有窗口卖票结束"<<std::endl;
}



/*
多线程编程-线程间的同步通信机制
多线程的两个问题
1，线程间的互斥
竞态条件-》临界区代码段-》原子操作-》互斥锁mutex 轻量级的无所实现
2，线程间的同步通信
生产者消费者线程模型
    //生产者生产一个物品，通知消费者消费一个，消费完了，消费者在通知生产者继续生产物品
 */

//原子操作
/*
互斥锁比较重，临界区代码做的事情比较复杂，多
系统理论：CAS来保证上面的++ --操作的原子特性就足够多了，无锁操作
    exchange/swap

 */