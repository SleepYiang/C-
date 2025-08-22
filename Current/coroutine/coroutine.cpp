//协程  :协程是和线程无关的，保存的东西是保存在对空间的，不会因为线程的结束而释放
//协程是可以暂停你和恢复的函数
//在c++ 中 co-await co_yield co_return 三个关键字中的其中一个就是协程
//co_await  用于暂停协程直到恢复
//我们promise::promise_type中的一个函数的返回类型即：std::suspend_never initial_suspend() noexcept {return{};}
//这个东西非常重要，能决定一个协程启动的时候，一开始是否暂停。
//协程开始的时候会调用promise.initial_suspend()并co_await他的结果。也就是等价于下面的代码
//std::suspend_always result=promise.initial_suspend();
//co_await result;
//因为await_read()返回true，所以不会暂停，这个函数的返回值，决定是否协程启动的时候暂停一下





//协程的相关知识
//协程是可以暂停和恢复的函数
//协程和线程的关系知识取决于协程是否跑在多线程上
#include<iostream>
#include<coroutine>
struct promise
{
    struct promise_type
    {
        promise get_return_object()
        {
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() noexcept{return {};}
        std::suspend_always final_suspend() noexcept{return {};}
        void return_void(){}
        void unhandled_exception(){}
    };
    std::coroutine_handle<promise_type>_h;
};
struct Input
{
    bool await_read()
    {
        return false;//false表示会暂停
    }
    void await_suspend(std::coroutine_handel<>h){}
    void await_resume(){};
};
int main()
{
    auto lambda=[]()->promise
    {
        Input t;
        std::cout<<"笑\n";
         std::cout<<"协程执行完毕\n";
    };
    promise result=lambda();
    std::cout<<"main()\n";
    result._h.resume();//回复协程
}
//协程相关的笔记
    //协程就是可以暂停和恢复的函数   暂停（性能需要 美观需要 逻辑需要）
    //协程可以暂停的函数
    //线程运行函数的地方
//c++20 相关的协程（可以在单线程中实现并发任务
    //