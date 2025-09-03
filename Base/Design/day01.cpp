#include<iostream>
#include <math.h>
using namespace std;
/*
单例模式：一个类不管拆功能键多少次对象，永远只能得到该类型的一个实例对象
饿汉式：创建单例后都完成初始化（没有实例化对象，实例就已经产生） 饿汉式一定是线程安全的
懒汉式：唯一的实例化对象，直到第一次获取他的时候才产生
常用到的 日志模块 数据库模块
 */
class Singleton
{
public:
    static Singleton* getInstance() //3.获取一个类的唯一实例对象接口方法
    {
        return &instance;
    }
private:
    static Singleton instance;  //2.定义一个唯一的类的实例对象
    Singleton(){}       //1.构造函私有化
    Singleton(Singleton const&)=delete;
    Singleton& operator=(Singleton const&)=delete;
};
int main()
{
    Singleton*p1=Singleton::getInstance();
    Singleton*p2=Singleton::getInstance();
    Singleton*p3=Singleton::getInstance();
    Singleton*p4=Singleton::getInstance();
    return 0;
}


//线程安全的饿汉式工作模式
