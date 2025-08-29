//智能指针的相关介绍
//实现智能智能
//不带引用计数的智能指针auto_ptr,scoped_ptr,unique_ptr
//带引用计数的智能指针 shared_ptr,weak_ptr
//之呢该指针的交叉引用（循环引用）问题
//多线程访问共享对象的问题
//自定义删除器
#include<iostream>
using namespace std;
//智能指针，保证能做到资源的自动释放
//利用栈上的对象出作用域自动析构的特征，来做到资源的自动释放
//智能指针是不可以直接放在对上
template<typename T>
class CSmartPtr
{
public:
    CSmartPtr(T *ptr=nullptr):mptr(ptr){}
    ~CSmartPtr(){delete mptr;}
    T&operator*(){return *mptr;}
    T* operator->(){return mptr;}
private:
    T*mptr;
};
int main()
{
    CSmartPtr<int> ptr(new int);
    *ptr=20;
    class Test
    {
    public:
        void test(){cout<<"call Test::test"<<endl;}
    };
    CSmartPtr<Test>ptr1(new Test());
    //(ptr1.operator->())->test();
    ptr1->test();//(*ptr1).test()
}
#if 0
int main1()
{
    //data段  heap堆   stack栈
    int *p=new int(10);
    *p=30;
    delete p;
    return 0;
}
#endif