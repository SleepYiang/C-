#include "ThreadPool.h"
#include<functional>
#include <iostream>
#include <ostream>
#include<thread>
const int TASK_MAX_THREHOLD=1024;
//线程池构造函数
ThreadPool::ThreadPool()
    :initThreadSize_(4)
    ,taskSize_(0)
    ,taskQueMaxThreadHold_(TASK_MAX_THREHOLD)
    ,poolMode_(PoolMode::MODE_FIXED)
{}
//线程池析构
ThreadPool::~ThreadPool(){}

//设置线程池的工作模式
void ThreadPool::setMode(PoolMode mode)
{
    poolMode_ = mode;
}

//设置任务队列阈值
void ThreadPool::setTaskQueMaxThreadHold(int threadhold)
{
    taskQueMaxThreadHold_ = threadhold;
}

//给线程池提交任务   用户调用该接口，传入任务对象
void ThreadPool::submitTask(std::shared_ptr<Task> sp)
{
    //获取锁
    std::unique_lock<std::mutex>lock(taskQueMtx_);
    //线程之间的通信 等待任务队列有空余
    //用户提交任务五不可已超过1秒，否则判定任务提交失败，返回
        //wait  一直等,直到了条件满足
        //wait_for  增加时间参数，最多等规定的时间
        //wait_until  增加时间参数，持续等待时间到什么时候
    if (!notFull_.wait_for(lock,std::chrono::seconds(1),
        [&]()-> bool{return taskQue_.size() < (size_t)taskQueMaxThreadHold_;}))
    {
        //表示notFull_等待1秒钟，体哦阿健依然没有满足
        std::cerr<<"task queue is full,submit task fail."<<std::endl;
        return ;
    }
    //如果队列有空余 把任务放入队列中
    taskQue_.emplace(sp);
    ++taskSize_;
    //因为新放入了任务，任务队列肯定不空，在not_empty上通知
    notFull_.notify_all();
}

//开启线程池
void ThreadPool::start(int initThreadSize)
{
    //记录初始线程个数
    initThreadSize_ = initThreadSize;

    //创建线程的对象
    for (int i=0;i<initThreadSize_;i++)
    {
        //创建thread线程对象的时候，把线程函数给到thread线程对象
        auto ptr=std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this));
        threads_.emplace_back(std::move(ptr));
    }

    //启动所有线程
    for (int i=0;i<initThreadSize_;i++)
    {
        threads_[i]->start();//需要去执行一个线程函数
    }
}
//定义线程函数
void ThreadPool::threadFunc()
{
    // std::cout<<"begin func tid: "<<std::this_thread::get_id()<<std::endl;
    // std::cout<<"end func tid: "<<std::this_thread::get_id()<<std::endl;
    for (;;)
    {
        std::shared_ptr<Task>task;
        {
            //先获取锁
            std::unique_lock<std::mutex> lock(taskQueMtx_);

            std::cout<<"tid: "<<std::this_thread::get_id()<<"尝试获取任务..."<<std::endl;
            //等待notempy条件
            notEmpty_.wait(lock, [&]()-> bool { return taskQue_.size() > 0; });
            std::cout<<"tid: "<<std::this_thread::get_id()<<"获取任务成功"<<std::endl;
            //从任务队列中获取一个任务出来
            auto task = taskQue_.front();
            taskQue_.pop();
            taskSize_--;
            //如果依然有任务，继续通知其他线程执行任务
            if (taskQue_.size()>0)
            {
                notEmpty_.notify_all();
            }
            //取出任务进行通知可以继续生产任务
            notFull_.notify_all();
        }//这里就释放锁
        //当前线程负责执行这个任务
        if (task!=nullptr)
        {
           task->run();
        }
    }
}
/**********************************线程方法实现***************************************/
//线程构造函数
Thread::Thread(ThreadFunc func)
    :func_(func)
{}
//线程析构函数
Thread::~Thread(){}
//启动线程
void Thread::start()
{
    //创建一个线程来执行一个线程函数
    std::thread t(func_);
    t.detach();     //设置分离线程
}