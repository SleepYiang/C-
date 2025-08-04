#include<iostream>
#include <memory>
#include<mutex>
#include <stack>
#include<thread>
#include <bits/this_thread_sleep.h>
std::mutex mtx1;
int shared_data = 100;
//创建互斥锁
void use_lock()
{
    while (true)
    {
        mtx1.lock();
        shared_data++;
        std::cout << "current thread id " << std::this_thread::get_id() << std::endl;
        std::cout << "shared data value " << shared_data << std::endl;
        mtx1.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

//守卫线程锁
void test_lock()
{
    std::thread t1(use_lock);
    std::thread t2([]()
    {
        while (true)
        {
            //添加作用域防止抱锁睡觉
            {
                std::lock_guard<std::mutex> lock(mtx1);
                shared_data--;
                std::cout << "current thread id " << std::this_thread::get_id() << std::endl;
                std::cout << "shared data value " << shared_data << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });
    t1.join();
    t2.join();
}
//如何保证数据的安全
template<typename T>
class threadsafe_stack1
{
public:
    threadsafe_stack1(){}
    threadsafe_stack1(const threadsafe_stack1&other)
    {
        std::lock_guard<std::mutex>lock(other.m);
        data = other.data;
    }
    threadsafe_stack1& operator=(const threadsafe_stack1& other)=delete;

    void push(T new_vale)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_vale);
    }
    //问题代码
    T pop()
    {
        std::lock_guard<std::mutex> lock(m);
        auto element= data.top();
        data.pop();
        return element;
    }
    //危险的方式
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
private:
    std::stack<T> data;
    mutable std::mutex m;//mutable 修饰变量可修改
};
//创造问题
//问题代码（当程序为只读是返回一个返回值而不加锁同意出现同时操作）
void test_threadsafe_stack1()
{
    threadsafe_stack1<int> safe_stack;
    safe_stack.push(1);
    std::thread t1([&safe_stack]()
    {
        if (!safe_stack.empty())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            safe_stack.pop();
        }
    });
    std::thread t2([&safe_stack]()
    {
        if (!safe_stack.empty())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            safe_stack.pop();
        }
    });
    t1.join();
    t2.join();
}
struct empty_stack:std::exception
{
    const char* what() const throw();
};
template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack&other)
    {
        std::lock_guard<std::mutex>lock(other.m);
        data = other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack& other)=delete;
    void push(T new_vale)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_vale);
    }
    //问题代码
    std::shared_ptr<T>pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();
            //return nullptr
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }
    void pop(T&value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();
        value=data.top();
        data.pop();;
    }
    //危险的方式
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
private:
    std::stack<T> data;
    mutable std::mutex m;
};
std::mutex t_lock1;
std::mutex t_lock2;
int m_1=0;
int m_2=1;
//死锁
void dead_lock1()
{
    while (true)
    {
        std::cout<<"dead_lock begin"<<std::endl;
        t_lock1.lock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        m_1++;
        t_lock2.unlock();
        m_2++;
        t_lock2.unlock();
        t_lock1.unlock();
        std::cout<<"dead_lock end"<<std::endl;
    }
}
void dead_lock2()
{
    while (true)
    {
        std::cout<<"dead_lock2 begin"<<std::endl;
        t_lock1.lock();
        m_1++;
        t_lock2.unlock();
        m_2++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        t_lock2.unlock();
        t_lock1.unlock();
        std::cout<<"dead_lock2 end"<<std::endl;
    }
}
void test_dead_lock()
{
    std::thread t1(dead_lock1);
    std::thread t2(dead_lock2);
    t1.join();
    t2.join();
}

//加锁和解锁作为原子操作解耦合，各自只管理自己的功能
void atomic_lock1() {
    std::cout << "lock1 begin lock" << std::endl;
    t_lock1.lock();
    m_1 = 1024;
    t_lock1.unlock();
    std::cout << "lock1 end lock" << std::endl;
}

void atomic_lock2() {
    std::cout << "lock2 begin lock" << std::endl;
    t_lock2.lock();
    m_2 = 2048;
    t_lock2.unlock();
    std::cout << "lock2 end lock" << std::endl;
}

void safe_lock1() {
    while (true) {
        atomic_lock1();
        atomic_lock2();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void safe_lock2() {
    while (true) {
        atomic_lock2();
        atomic_lock1();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void test_safe_lock() {
    std::thread t1(safe_lock1);
    std::thread t2(safe_lock2);
    t1.join();
    t2.join();
}

//对于要使用两个互斥量，可以同时加锁
//如不同的时加锁可能会死锁

//假设式复杂的数据结构
class som_big_object
{
public:
    som_big_object(int data):_data(data){}
    //拷贝构造
    som_big_object(const som_big_object&other):_data(other._data){}
    //移动构造
    som_big_object(som_big_object&&other):_data(std::move(other._data)){}
    //重载输出运算符
    friend std::ostream& operator<<(std::ostream&os,const som_big_object& obj)
    {
        os<<obj._data;
        return os;
    }
    //重载赋值运算符
    som_big_object& operator=(const som_big_object& other)
    {
        if (this == &other)
            return *this;
        _data = other._data;
        return *this;
    }

    //实现数据交换
    friend void swap(som_big_object&a,som_big_object&b)
    {
        som_big_object temp=std::move(a);
        a=std::move(b);
        b=std::move(temp);
    }
private:
    int _data;
};
//假设这是一个交换的结构，包含了复杂的成员对象和一个互斥量
class big_object_mgr {
public:
    big_object_mgr(int data = 0) :_obj(data) {}
    void printinfo() {
        std::cout << "current obj data is " << _obj << std::endl;
    }
    friend void danger_swap(big_object_mgr& objm1, big_object_mgr& objm2);
    friend void safe_swap(big_object_mgr& objm1, big_object_mgr& objm2);
    friend void safe_swap_scope(big_object_mgr& objm1, big_object_mgr& objm2);
private:
    std::mutex _mtx;
    som_big_object _obj;
};
void danger_swap(big_object_mgr& objm1, big_object_mgr& objm2) {
    std::cout << "thread [ " << std::this_thread::get_id() << " ] begin" << std::endl;
    if (&objm1 == &objm2) {
        return;
    }
    std::lock_guard <std::mutex> gurad1(objm1._mtx);
    //此处为了故意制造死锁，我们让线程小睡一会
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> guard2(objm2._mtx);
    swap(objm1._obj, objm2._obj);
    std::cout << "thread [ " << std::this_thread::get_id() << " ] end" << std::endl;
}
void  test_danger_swap() {
    big_object_mgr objm1(5);
    big_object_mgr objm2(100);

    std::thread t1(danger_swap, std::ref(objm1), std::ref(objm2));
    std::thread t2(danger_swap, std::ref(objm2), std::ref(objm1));
    t1.join();
    t2.join();

    objm1.printinfo();
    objm2.printinfo();
}

void safe_swap(big_object_mgr& objm1, big_object_mgr& objm2) {
    std::cout << "thread [ " << std::this_thread::get_id() << " ] begin" << std::endl;
    if (&objm1 == &objm2) {
        return;
    }
    //std::lock可以同时加两个锁
    std::lock(objm1._mtx, objm2._mtx);
    //领养锁管理它自动释放（不用管理他的加锁和初始化，只需要管理他的释放）
    std::lock_guard <std::mutex> gurad1(objm1._mtx, std::adopt_lock);

    //此处为了故意制造死锁，我们让线程小睡一会
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::lock_guard <std::mutex> gurad2(objm2._mtx, std::adopt_lock);

    swap(objm1._obj, objm2._obj);
    std::cout << "thread [ " << std::this_thread::get_id() << " ] end" << std::endl;
}
//上述代码可以简化为以下方式
void safe_swap_scope(big_object_mgr& objm1, big_object_mgr& objm2) {
    std::cout << "thread [ " << std::this_thread::get_id() << " ] begin" << std::endl;
    if (&objm1 == &objm2) {
        return;
    }

    std::scoped_lock  guard(objm1._mtx, objm2._mtx);
    //等价于
    //std::scoped_lock<std::mutex, std::mutex> guard(objm1._mtx, objm2._mtx);
    swap(objm1._obj, objm2._obj);
    std::cout << "thread [ " << std::this_thread::get_id() << " ] end" << std::endl;
}
void test_safe_swap() {
    big_object_mgr objm1(5);
    big_object_mgr objm2(100);

    std::thread t1(safe_swap, std::ref(objm1), std::ref(objm2));
    std::thread t2(safe_swap, std::ref(objm2), std::ref(objm1));
    t1.join();
    t2.join();

    objm1.printinfo();
    objm2.printinfo();
}
//层级锁
class hierarchical_mutex
{
public:
    explicit hierarchical_mutex(unsigned value):_hierarchy_value(value),
    _previous_hierarchy_value(0){}
    hierarchical_mutex(const hierarchical_mutex &other)=delete;
    hierarchical_mutex& operator=(const hierarchical_mutex &other)=delete;
    void lock()
    {
        check_for_hirerachy_violation();
        _internal_mutex.lock();
        update_hierarchy_violation();
    }
    void unlock()
    {
        if (_this_thread_hierarchy_value!=_hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
        _this_thread_hierarchy_value=_hierarchy_value;
        _internal_mutex.unlock();
    }
    bool try_lock()
    {
        check_for_hirerachy_violation();
        if (!_internal_mutex.try_lock())
        {
            return false;
        }
        update_hierarchy_violation();
        return true;
    }
private:
    std::mutex _internal_mutex;
    //当前层级值
    unsigned long const _hierarchy_value;
    //上一级层级值
    unsigned long _previous_hierarchy_value;
    //本线程记录的层级值
    static thread_local unsigned long _this_thread_hierarchy_value;
    void check_for_hirerachy_violation()
    {
        if (_this_thread_hierarchy_value<=_hierarchy_value)
        {
            throw std::logic_error("hierarchy violation");
        }
    }
    void update_hierarchy_violation()
    {
        _previous_hierarchy_value = _this_thread_hierarchy_value;
        _this_thread_hierarchy_value = _hierarchy_value;
    }
};
thread_local unsigned long hierarchical_mutex::_this_thread_hierarchy_value(ULLONG_MAX);
void test_hierarchy_lock()
{
    hierarchical_mutex hmtx1(1000);
    hierarchical_mutex hmtx2(500);
    std::thread t1([&hmtx1, &hmtx2]()
    {
        hmtx1.lock();
        hmtx2.lock();
        hmtx2.unlock();
        hmtx1.unlock();
    });
    std::thread t2([&hmtx1, &hmtx2]()
    {
        hmtx2.lock();
        hmtx1.lock();
        hmtx1.unlock();
        hmtx2.unlock();
    });
}
int main()
{
    //test_lock();
    //test_threadsafe_stack1();
    //test_dead_lock();
    //test_safe_lock();
    //test_danger_swap();
    test_hierarchy_lock();
    return 0;
}
//shuiyi
