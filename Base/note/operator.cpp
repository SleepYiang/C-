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
int main()
{
    String str1;
    String str2="aaa";
    String str3="bbb";
    String str4=str3+str2;
    cout<<str4;
    return 0;
}