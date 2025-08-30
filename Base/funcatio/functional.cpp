#include<string>
#include<iostream>
#include<functional>
#include<typeinfo>
using namespace std;
//function函数对象类型的实现原理
#if 0
void hello(string str){cout<<str<<endl;}
template<typename Fty>
class myfunction{};

template<typename R,typename A1>
class myfunction<R(A1)>
{
public:
    using PFUNC=R(*)(A1);
    myfunction(PFUNC pfunc):pfunc_(pfunc){}
    R operator()(A1 arg)
    {
        return pfunc_(arg);
    }
private:
    PFUNC pfunc_;
};
int main()
{
    myfunction<void(string)>func1(hello);
    func1("hello world!");
    return 0;
}
#endif

/*
 * C++ bing绑定器  -》返回结果还是一个函数对象
 */
void hello(string str){cout<<str<<endl;}
int sum(int a, int b){return a+b;}
class Test
{
public:
    int sum(int a,int b)
    {
        return a+b;
    }
};
int main()
{
    //bind是函数模板，可以自动推演模板类型参数
    bind(hello,"hello")();
    cout<<bind(sum,10,20)()<<endl;
    cout<<bind(&Test::sum,Test(),20,30)()<<endl;

    //参数占位符(bind最多可以使用20个占位符)

    bind(hello,placeholders::_1)(" hello bind 2");

    //绑定器出了语句，无法继续使用
    //通过function函数把绑定其bind就复用起来了
    function<void(string)>func1=bind(hello,placeholders::_1);
    func1("hello bind 3");
    func1("hello bind 4");
    func1("hello bind 5");

    return 0;
}