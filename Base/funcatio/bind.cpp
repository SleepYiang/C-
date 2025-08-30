#include<iostream>
#include<vector>
#include<map>
#include<functional>    //使用functional函数对象类型
#include<ctime>
#include<algorithm>
#include <iostream>
#include <ostream>
#include<string>
using namespace std;
/*
 *1.用函数类型实例化function
 * 2.通过function调用operator()函数的时候，需要根据函数类型出入相应的参数
 * c++提供绑定其器和函数对象
 *  bind dunction
 *  c++ STL bind1st和bind2nd ->本身哈四世一个函数对象
 *  function：绑定器，函数对象 lambad表达式 他们只能使用在一条语句中
 */
void hello(string str)
{
    cout<<str<<endl;
}
void hello1()
{
    cout<<"hello world"<<endl;
}
int sum(int a, int b)
{
    return a+b;
}
class Test
{
public://调用成员方法必须依赖一个对象 void（Test：：*pfunc）(string)
    void hello(string str)
    {
        cout<<str<<endl;
    }
};
int main()
{
    //从function的模板定义处，看到希望用一个函数类型实例化function
    function<void()> func1(hello1);
    func1();//func1.operator()->hello1()

    function<void (string)>func2=hello;
    func2("hello hello");//func2.operator(string str)->hello(string)

    function<int(int,int)>func3=sum;
    cout<<func3(3,4)<<endl;

    function<void(Test*,string)>func4=&Test::hello;
    func4(&Test(),"call Test::helo");
    return 0;
}