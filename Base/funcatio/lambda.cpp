#include<iostream>
#include<algorithm>
#include <functional>
#include <map>
#include<memory>
using namespace std;
/*
 * C++函数对象的升级版-》lambda表达式
 * 函数对象的缺点
 * 使用泛型算法参数传递   比较性质/自定义操作  优先级队列
 *
 * lambda表达式的语法
 * [捕获外部变量]（形参列表）->返回值{操作代码}
 *
 * 如果lambda的返回值是不需要的，那么“->”返回值是可以省略的
 *
 * [捕获变量]
 * []表示不捕获任何变量
 * [=]以传值的方式捕获外部的所有变量
 * [&]以传引用的方式捕获外部变量
 * [this]捕获外部的this指针
 * [=,&a]以传值的方式捕获外部的所有变量，但是a变量以传引用的方式捕获
 */
#if 0
int main()
{
    auto func1=[]()->void {cout<<"hello world"<<endl;};
    func1();

    auto func2=[](int a,int b)->int {return a+b;};
    cout<<func2(1,2)<<endl;

    return 0;
}
#endif

//既然lambda表达式只能使用在语句当中，如果想跨语句使用之前定义好的lambda表达式，怎么办，用什么类型表示
    //lambda 表达式->函数对象
    //使用function类型来表示函数对象的类型

int main()
{
    map<int,function<int(int,int)>>caculateMap;
    caculateMap[1]=[](int a,int b)->int {return a+b;};
    caculateMap[2]=[](int a,int b)->int {return a-b;};
    caculateMap[3]=[](int a,int b)->int {return a*b;};
    caculateMap[4]=[](int a,int b)->int {return a/b;};
    cout<<"10+15"<<caculateMap[1](10,25)<<endl;
    return 0;
}