#include <cstring>
#include<iostream>
#include<string>
#include<functional>
using namespace std;
/*
 * 模板的完全特例化和非完全（部分）特例化
 * 模板的实参推演
 *
 *在模板中有符合的特例化就走特例化，如果没有符合的类型就走基本的模板
 */
#if 0
template<typename T>
bool compare(T a,T b)
{
    cout<<"template compare"<<endl;
    return a>b;
}
template<>
bool compare<const char*>(const char* a,const char* b)
{
    cout<<"template compare<const char*>"<<endl;
    return strcmp(a,b)>0;
}
int main()
{
    compare(10,20);
    compare("aaa","bbb");// T const char*
    return 0;
}
#endif

//function函数对象类型的实现原理
