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
