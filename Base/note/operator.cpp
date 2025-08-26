//运算符重载:使对象运算表现的和编译器内置类型一样
//编译器做对象运算的时候，会调用运算符重载函数，优先调用成员方法，如果没有成员方法，就在全局作用域内寻找合适的运算符重载函数
//单目运算符重载    operator++()//前置++    operator++(int)//后置++
#include <cstring>
#include<iostream>
#include<string>
using namespace std;
class String
{
public:
    String(const char * p=nullptr)
    {
        if (p!=nullptr)
        {
            pstr_=new char[strlen(p)+1];
            strcpy(pstr_, p);
        }else
        {
            pstr_=new char[1];
            *pstr_='\0';
        }
    }
    ~String()
    {
        delete [] pstr_;
        pstr_=nullptr;
    }
    String(const String &str)
    {
        pstr_=new char[strlen(str.pstr_)+1];
        strcpy(pstr_, str.pstr_);
    }
    String & operator=(const String &str)
    {
        if (this==&str)
            return *this;
        delete [] pstr_;
        pstr_=new char[strlen(str.pstr_)+1];
        strcpy(pstr_, str.pstr_);
        return *this;
    }
    //给字符串类型提供迭代器
    class iterator
    {
    public:
    private:
        char * p_;
    };
    //begin返回容器底层元素的迭代器的标志
    iterator begin(){return iterator(pstr_);    }
    iterator end();
private:
    char*pstr_;
    friend ostream& operator<<(ostream &out,const String &str);
    friend String operator+(const String &str1, const String &str2);
};
String operator+(const String &str1, const String &str2)
{
    char *ptmp=new char[strlen(str1.pstr_)+strlen(str2.pstr_)+1];
    strcpy(ptmp, str1.pstr_);
    strcat(ptmp, str2.pstr_);
    return String(ptmp);
}
ostream& operator<<(ostream &out,const String &str)
{
    out<<str.pstr_<<endl;
}
//泛型算法参数接受的都是迭代器
int main()
{
    string str1="hello world";//str1
    //容器的迭代器
    string::iterator it=str1.begin();//返回首元素的地址
    for (;it!=str1.end();it++)
    {
        cout<<*it<<endl;//访问容器内部元素的值
    }
    // String str1;
    // String str2="aaa";
    // String str3="bbb";
    // String str4=str3+str2;
    // cout<<str4;
    // return 0;
}


//new和delete
//new和malloc的区别
//1.malloc按字节开辟内存，new开辟内存的时候需要指定类型 new int[]
    //maloc开辟内训的返回的是void* operator new->int*
//2.malloc只负责开辟空间，new不仅要开辟空间还需要初始化
//3.malloc开辟失败返回nullptr，new则是抛出异常

//free和delete的区别
//1.delete :先调用析构再free

//new和delete可以混用吗？

//c++为什么区分单个元素和数组类型的?
//自定义类类型，有析构函数，为了正确调用析构函数，哪呢开辟对象数组的时候，会多开辟4个字节，记录对象的个数

//继承扽本质和原理
//继承的本质 1.代码复用 2.
    //类和类之间的关系  组合： a part of (一部分的关系)    组合： a kind of(一种关系)
/*
    继承方式            基类的访问限定       派生类的访问限定     （mian）外部的访问限定
    public               public             public                  ok
                         private            不可见的                 no
                         protected          protected               no
    protected   (基类成员的访问限定，在派生类里面是不可能超过继承方式的)
                         public             protedted               ok
                         private            不可见的                 no
                         protected          protected               no
    private
                         public             protedted               no
                         private            不可见的                 no
                         protected          private                 no
 */
//1.外部只可以访问对象public的成员，protected和private的成员无法直接访问
//2.在继承结构中，派生类从基类可以继承过来private的成员，但是派生类却无法直接访问
//3.protect和private的区别？  在基类中定义的成员想被派生类访问不想让外部访问那么在基类中定义为protected

//派生类的构造过程
//派生类如何初始化从基类继承来的成员变量呢
//1.派生类从继承可以继承所有来的成员（变量和方法），除过构造函数和析构函数

//派生类如何初始化从基类继承来的成员变量呢？  调用基类相应的构造函数来初始化

//派生类分构造函数和析构函数，负责初始化和清理派生类部分
//派生类从基类继承来的成员初始化和清理由谁负责？ 是由基类的构造函数和析构函数负责

//派生对象构造和析构过程
//1.派生类调用基类的构造函数，初始化从基类继承来的成员
//2.调用派生类自己的构造函数，初始化派生类自己特有的成员

//重载 隐藏 覆盖
//1.重载关系
    //一组函数要重载，必须处于同一个作用域中，而且函数的名字相同，参数列表不同
//2，隐藏（作用域）关系
    //在继承结构中，派生类同名的成员，把基类同名成员给隐藏调用了（派生类和基类成员同名调用派生类的）
//3.覆盖（虚函数表中地址的覆盖  ）
    //激烈和派生类的方法，返回值，函数名以及参数列表都子相同，而且基类的方法都是虚函数，那么派生类的方法就自动处理成虚函数，他们之间成为覆盖

//把继承机构，也说成从上（基类）到下（派生类）的结构
    //基类对象 <-派生类对象  从下到上的转换（可以）
    //派生类对象<-基类对象   从上到下转换（不可以）
    //基类指针（引用）<-派生类对象 从下到上的转换（可以）
    //派生类指针（引用）<-基类对象 从上到下转换（不可以）
//总结：在继承结构中进行上下的类型转换，默认只支持从下到上的类型转换

//虚函数   静态绑定和动态绑定
//总结一，如果类里面定义了虚函数，那么编译阶段，编译器给这个类型产生一个唯一的vftable虚函数表，虚函数表中主要存储的内容就是RTTI
    //指针和虚函数的地址。（RTTI运行时的类型信息）当程序运行时，每一张虚函数表，夺回加载到内存的.rodata区
//总结二，一个类里面定义了一个函数，那么这个类定义的对象，其运行时，内存中开始的部分多存储一个vfptr虚函数指针，指向相应类型的虚函数表vftable
    //一个类型定义n个对象，他们的vfpt指向的都是同一张虚函数表
//总结三，一个类里面的虚函数个数，不影响对象的内存大小（vfptr）影响的是虚函数表的大小
//总结四，如果派生类中的方法，和基类继承来的某个方法，返回值函数名参数列表都相同，而且基类的方法virtual虚函数，那么派生类的这个方法
//自动处理成虚函数


//如何理解多态
//编译时期的多态：函数重载 模板（函数模板，类模板）
//运行时期的多态
    //在继承结构中，基类指针（引用）指向派生类对象，通过指针（引用）调用同名覆盖方法（虚函数）
    //基类指针指向那个派生类对象，就会第哦啊用哪个派生类的覆盖方法，成为多态
    //多态底层是通过动态绑定来实现的

//多重继承
//虚基类/抽象类（有纯虚函数的类）

//virtual
//修饰成员方法是虚函数
//可以修饰继承方式，是虚继承，被继承的类，称为虚基类

//菱形继承
//会导致成员重复继承导致内存的浪费

//c++语言级别提供的四种类型转换
//const_cast   把常量属性去掉
//static_cast   提供编译器认为安全的类型转化按（没有任何联系的类型之间的转换就被否定了 ）
//reinterpret_cast  //类似于C风格的类型转换
//dynamic_cast      //主要用在继承的结构中，可以支持RTTI类型识别的上下转换
