//********************************对象的应用与优化，右值引用和优化********************************************
#include<iostream>
using namespace std;
class Test
{
public:
    Test(int a=10):ma(a)
    {
        cout<<"Test(int)"<<endl;
    }
    ~Test()
    {
        cout<<"~Test()"<<endl;
    }
    Test(const Test &t):ma(t.ma)
    {
        cout<<"Test(const Test &t)"<<endl;
    }
    Test&operator=(const Test&t)
    {
        cout<<"Test operator= (const Test &t)"<<endl;
        ma = t.ma;
        return *this;
    }
private:
    int ma;
};
int main()
{
    Test t1;
    Test t2(t1);
    Test t3=t1;
    //Test(20)显示生成临时对象，生命周期；所在的语句
    //c++编译器对于对象构造的优化：用临时对象生成新对象的时候，临时对象就不产生，直接构造新的对象就可以了
    Test t4=Test(20);//Test4（20） 没有区别
    cout<<"_____________________________"<<endl;
    //t4.operator=(t2)
    t4=t2;
    //t4.operator=(const Test &t)
    //显示生成临时对象
    t4=Test(30);
    //隐式生成临时对象
    t4=(Test)30;    //Test（30） int->Test(int)
    cout<<"_____________________________"<<endl;
    //Test *p= &Test(40);
    //p指向的是一个已经析构的临时对象
    const Test &ref=Test(50);
    //结论 用指针指向临时对象是不安全的，但是引用对象就可以
    cout<<"_____________________________"<<endl;
    return 0;
}
//**************************************对象的优化
//不能返回局部的或者临时的指针或者引用
//1.函数参数传递过程中，对象优先按引用传递，不要按值传递
//2.函数返回对象的时候，应该优先返回一个临时对象，而不要返回一个定义过的对象
//3.接受返回值是对象的函数调用的时候，优先按初始化的方式接收，不要按赋值的方式接受