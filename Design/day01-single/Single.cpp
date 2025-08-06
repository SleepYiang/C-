#include<iostream>
#include<mutex>
#include<memory>
//懒汉式不安全版
class SingleLazy
{
public:
    static SingleLazy& getInstance()
    {
        static SingleLazy  Single;
        return Single;
    }
private:
    //私有化构造函数
    SingleLazy(){};
    SingleLazy(const SingleLazy&)=delete;
    SingleLazy& operator=(const SingleLazy&)=delete;
};
void testSingleLazy()
{
    std::cout<<"SingleLazy1 address "<<&SingleLazy::getInstance()<<std::endl;
    std::cout<<"SingleLazy2 address "<<&SingleLazy::getInstance()<<std::endl;
}
//线程安全的懒汉模式
class SingleLazy_safethread
{
public:
    static SingleLazy_safethread* GetInst()
    {
        if (single!=nullptr)
        {
            return single;
        }
        mtx_.lock();
        if (single!=nullptr)
        {
            mtx_.unlock();
            return single;
        }
        single = new SingleLazy_safethread();
        mtx_.unlock();
        return single;
    }
private:
    SingleLazy_safethread(){}
    SingleLazy_safethread(const SingleLazy_safethread&)=delete;
    SingleLazy_safethread& operator=(const SingleLazy_safethread&)=delete;
    static std::mutex mtx_;
    static SingleLazy_safethread* single;
};

//利用智能指针实现懒汉安全
class SingleAuto
{
private:
    SingleAuto(){}
    SingleAuto(const SingleAuto&) = delete;
    SingleAuto& operator=(const SingleAuto&) = delete;
public:
    ~SingleAuto()
    {
        std::cout << "single auto delete success " << std::endl;
    }
    static std::shared_ptr<SingleAuto> GetInst()
    {
        if (single != nullptr)
        {
            return single;
        }
        s_mutex.lock();
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = std::shared_ptr<SingleAuto>(new SingleAuto);
        s_mutex.unlock();
        return single;
    }
private:
    static std::shared_ptr<SingleAuto> single;
    static std::mutex s_mutex;
};

std::shared_ptr<SingleAuto> SingleAuto::single = nullptr;
std::mutex SingleAuto::s_mutex;
void test_singleauto()
{
    auto sp1 = SingleAuto::GetInst();
    auto sp2 = SingleAuto::GetInst();
    std::cout << "sp1  is  " << sp1 << std::endl;
    std::cout << "sp2  is  " << sp2 << std::endl;
}
int main()
{
    //懒汉式会出现的问题是一在使用时才会初始化但是在使用时会出现重读初始化的文艺（一个正在初始化但是另一个以为没有就还在初始化）
    //testSingleLazy();
    //利用互斥锁实现线程安全保证不会多次初始化
    //testSingleLazy_safe();
    test_singleauto();
    return 0;
}