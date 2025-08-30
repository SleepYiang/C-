#include<iostream>
#include<memory>
using namespace std;
/*
    shared_ptr:强智能指针   可以改变资源的引用计数
    weak_ptr:弱智能指针      不会改变资源的引用计数（只能观察资源）
    weak_ptr=>shared_ptr =>资源（内存）

    强智能指针循环引用（交叉引用）是什么问题？什么结果？怎么解决？
    造成new出来的资源无法释放，出现资源泄露的问题
    定义对象的时候 使用强智能指针 引用对象的地方   使用弱智能指针
 */
class B;
class A
{
public:
    A(){cout<<"A()"<<endl;}
    ~A(){cout<<"~A()"<<endl;}
    shared_ptr<B> ptrb_;
};
class B
{
    public:
    B(){cout<<"B()"<<endl;}
    ~B(){cout<<"~B()"<<endl;}
    shared_ptr<A> ptra_;
};
int main()
{
    shared_ptr<A> pA(new A());
    shared_ptr<B> pB(new B());
    pA->ptrb_=pB;
    pB->ptra_=pA;
    cout<<pA.use_count()<<endl;
    cout<<pB.use_count()<<endl;
    return 0;
}

//多线程访问共享对象的线程安全问题
    //强智能指针，弱智能指针

//智能指针删除器   deletor
//智能指针   能够保证资源的绝对释放 delete ptr;
