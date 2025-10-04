## RAII
**RAII**:资源获取即初始化
> * 构造函数中申请分配资源，在析构函数中释放资源
> * RAII的核心思想和就是将资源或者状态信息和生命周期的绑定
## 信号量
**信号量**是一种特殊的操作，他有两种操作一种是P操作,一种是V操作
> * P操作，当信号量SV大于0的时候则将其减一，若SV值为0，则挂起等待
> * V操作，当信号量SV如果由其他的几次呢还给你因为等待而挂起则恢复，若没有则加一

> * sem_init初始化一个未命名的信号量
> * sem_destory销毁信号量
> * sem_wait将以原子操作的方式对信号量减一，当信号量为0的时候，sem_wait阻塞
> * sem_post将以原子操作的方式对信号量加一，当信号量大于0时，唤醒调用sem_post线程

## 本小结所用到的相关笔记
### c++20信号量 
> * acquire() 阻塞式获取等同于sem_wait
> * release() 释放资源等同于sem_post
> * std::counting_semaphore<> sem() 初始化信号量<>()中填写信号量的数目

**信号量和互斥锁的区别**:**二进制信号量**信号量的数目为0,1,功能和互斥锁的功能相同，可以实现互斥量的访问。

**计数信号量**主要是式用于限制访问例如限制线程数等
```c++
#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>  // C++20 信号量
#include <chrono>
// 信号量：限制最大并发数为 3
std::counting_semaphore<3> sem(3);  // 初始计数 3，最大计数 3
// 线程任务：访问共享资源
void access_resource(int thread_id) {
    // 等待信号量（计数-1，若计数为0则阻塞）
    sem.acquire();  
    std::cout << "线程 " << thread_id << " 开始访问资源（当前并发数：" << 3 - sem.max() + sem.current() << "）\n";
    // 模拟资源访问（耗时操作）
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // 释放信号量（计数+1，唤醒等待线程）
    sem.release();  
    std::cout << "线程 " << thread_id << " 结束访问资源\n";
}
int main() {
    const int thread_count = 5;  // 创建 5 个线程（超过最大并发数 3）
    std::vector<std::thread> threads;
    // 启动所有线程
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(access_resource, i);
    }
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
```
输出的结果
```
线程 0 开始访问资源（当前并发数：1）
线程 1 开始访问资源（当前并发数：2）
线程 2 开始访问资源（当前并发数：3）
线程 0 结束访问资源
线程 3 开始访问资源（当前并发数：3）
线程 1 结束访问资源
线程 4 开始访问资源（当前并发数：3）
线程 2 结束访问资源
线程 3 结束访问资源
线程 4 结束访问资源
```
### explicit
`explicit` 关键字主要用于修饰单参数构造函数（或只有第一个参数无默认值、其余参数有默认值的构造函数），其核心作用是**禁止隐式类型转换**，避免意外的类型转换导致逻辑错误。

示例：无 explicit 时的隐式转换（可能引发问题）
```c++
class MyString {
public:
    // 单参数构造函数：接收 const char* 初始化字符串
    MyString(const char* str) {
        // 实现省略（假设内部存储字符串）
    }
};

void printString(MyString s) {
    // 打印字符串
}

int main() {
    MyString s1 = "hello";  // 隐式转换："hello"（const char*）→ MyString
    printString("world");   // 隐式转换："world" → MyString，作为参数传递
    return 0;
}
```
示例：加 explicit 后禁止隐式转换
```c++
class MyString {
public:
    // 用 explicit 修饰单参数构造函数
    explicit MyString(const char* str) {
        // 实现省略
    }
};

void printString(MyString s) {}

int main() {
    MyString s1 = "hello";  // 错误！禁止隐式转换
    printString("world");   // 错误！禁止隐式转换为 MyString 作为参数

    // 必须显式调用构造函数（允许）
    MyString s2("hello");
    printString(MyString("world"));  // 显式转换，允许
    return 0;
}
```
### 条件变量
**条件变量**:条件变量（std::condition_variable）是多线程同步的核心工具之一，用于实现线程间的**等待-通知机制**：一个线程可以等待某个条件满足，而其他线程在条件满足时唤醒等待的线程。

条件变量本身不管理数据，而是协调线程的执行顺序。其核心逻辑是：
* **等待方**：线程获取互斥锁后，检查条件是否满足。若不满足，则释放锁并进入阻塞状态（等待），直到被唤醒。
* **通知方**：当条件满足时，线程通知条件变量，唤醒一个或所有等待的线程，让它们重新检查条件并继续执行。