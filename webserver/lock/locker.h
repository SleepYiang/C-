#ifndef WEBSERVER_LOCKER_H
#define WEBSERVER_LOCKER_H
#include<mutex>
#include<thread>
#include<exception>
#include<condition_variable>
#include<semaphore>

//信号量的封装
class sem
{
public:
    // 构造函数：初始化信号量计数
    explicit sem(size_t initial_count = 0)
        : sem_(initial_count){}
    // 禁用拷贝构造和赋值（信号量不可复制）
    sem(const sem&) = delete;
    sem& operator=(const sem&) = delete;

    // 等待信号量（计数减 1，若为 0 则阻塞）
    void wait()
    {
        sem_.acquire(); // 替代 sem_wait
    }

    // 发送信号（计数加 1，唤醒等待线程）
    void post()
    {
        sem_.release(); // 替代 sem_post
    }

private:
    std::counting_semaphore<> sem_;
};

//互斥所的封装
class locker
{
public:
    locker()=default;
    //锁不可以拷贝构造和赋值
    locker(const locker&) = delete;
    locker& operator=(const locker&)=delete;
    //加锁
    void lock()
    {
        mutex_.lock();
    }
    //解锁
    void unlock()
    {
        mutex_.unlock();
    }
    std::mutex *get()
    {
        return &mutex_;
    }
private:
    std::mutex mutex_;
};

//条件变量
class cond
{
public:
    cond()=default;
    cond(const cond&)=delete;
    cond& operator=(const cond&)=delete;
    void wait(std::unique_lock<std::mutex>&lock)
    {
        cv_.wait(lock);
    }
    //超时等待
    template <typename Rep, typename Period>
    bool wait_for(std::unique_lock<std::mutex>& lock,
               const std::chrono::duration<Rep, Period>& timeout)
    {
        return cv_.wait_for(lock, timeout) == std::cv_status::no_timeout;
    }
    //通知一个线程
    void signal()
    {
        cv_.notify_one();
    }
    //通知所有线程
    void broadcast()
    {
        cv_.notify_all();
    }
private:
    std::condition_variable cv_;
};

#endif //WEBSERVER_LOCKER_H
