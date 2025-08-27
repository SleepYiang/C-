#ifndef TASKMRG_THREADPOOL_H
#define TASKMRG_THREADPOOL_H
#include<vector>
#include<queue>
#include<memory>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<functional>
//Any类型的实现
class Any
{
public:
    Any()=default;
    ~Any()=default;
    Any(const Any&)=delete;
    Any& operator=(const Any&)=delete;
    Any(Any&&)=default;

    //这个构造函数可以让any类型接受任意其他类型的数据
    template<typename T>
    Any(T data):base_(std::make_unique<Derive<T>>(data)){}
    //把any对象里存储的数据提取出来
    template<typename T>
    T cast_()
    {
        //我们怎么从base找到他所指向的derive对象，从他里面取出data成员变量
        //基类指针转换为派生类指针
        Derive<T>*pd=dynamic_cast<Derive<T>*>(base_.get());
        if(pd==nullptr)
        {
            throw "type is unmatch";
        }
        return pd->data_;
    }
private:
    //基类类型
    class Base
    {
    public:
        virtual ~Base()=default;
    };
    //派生类类型
    template<typename T>
    class Derive:public Base
    {
    public:
        Derive(T data):data_(data){}
        T data_; //保存了任意的其他类型
    };
private:
    //定义一个基类的指针
    std::unique_ptr<Base>base_;
};

//任务抽象基类
class Task
{
public:
    //用户自定义任意任务类型，从Task继承,重写run方法，实现自定义任务处理
    virtual void run()=0;
};

//线程支持的模式
enum class PoolMode
{
    MODE_FIXED,         //固定数量的线程
    MODE_CACHED,        //线程数量可动态增长
};
//线程类型
class Thread
{
public:
    //线程函数对象类型
    using ThreadFunc = std::function<void()>;
    //线程构造函数
    Thread(ThreadFunc func);
    //线程析构函数
    ~Thread();
    //启动线程
    void start();
private:
    ThreadFunc func_;
};
/*
    example:
    ThreadPool pool;
    pool.start(4);

    class My_task
    {
    public;
    void run()
    };
    pool.submitTask(std::make_shared<MyTask>());
 */
//线程池类型
class ThreadPool
{
public:
    ThreadPool();//构造
    ~ThreadPool();//析构
    ThreadPool(const ThreadPool&) = delete;//拷贝构造
    ThreadPool& operator=(const ThreadPool&) = delete;//拷贝赋值
    //开启线程池
    void start(int initThreadSize=4);
    //设置线程池的工作模式
    void setMode(PoolMode mode);

    //设置任务队列阈值
    void setTaskQueMaxThreadHold(int threadhold);
    //给线程池提交任务
    void submitTask(std::shared_ptr<Task> sp);
private:
    //定义线程函数
    void threadFunc();

private:
    std::vector<std::unique_ptr<Thread>> threads_;    //线程列表
    size_t initThreadSize_;           //初始化线程数量

    std::queue<std::shared_ptr<Task>> taskQue_;//任务队列
    std::atomic_int taskSize_;                 //任务数量
    int taskQueMaxThreadHold_;                 //任务队列数量的阈值
    std::mutex taskQueMtx_;                    //保证任务队列的线程安全
    std::condition_variable notFull_;          //表示任务线程不满
    std::condition_variable notEmpty_;         //表示任务线程不空
    PoolMode poolMode_;                        //线程池的工作模式
};
#endif //TASKMRG_THREADPOOL_H