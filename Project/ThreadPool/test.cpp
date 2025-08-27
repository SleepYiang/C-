#include<iostream>
#include <thread>
#include"ThreadPool.h"

/**
    有些场景，是希望能够获取线程执行任务返回的值
    举例：
    1+.........30000
 */
class MyTask:public Task
{
public:
    MyTask(int begin ,int end):begin_(begin),end_(end){}
    //问题一：怎么设计run的返回值，可以标准是任意的类型
    //c++17 Any类型
    Any run()
    {
        std::cout<<"tid: "<<std::this_thread::get_id()<<"begin"<<std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(2));
        int sum=0;
        for(int i=begin_;i<=end_;i++)
        {
            sum=sum+i;
            std::cout<<"tid: "<<std::this_thread::get_id()<<"end"<<std::endl;
            return sum;
        }
    }
private:
    int begin_;
    int end_;
};
int main()
{
    ThreadPool pool;
    pool.start(4);
    //如何设计这里的Result机制
    Result res=pool.submitTask(std::make_shared<MyTask>());
    res.get().cast<int>();         //get返回了一个any类型，怎么准换成其他类型
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    getchar();
}