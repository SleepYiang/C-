#include<iostream>
#include<string>
using namespace std;
template<class T>
void show(T t)
{
    cout<<t<<endl;
}
int main()
{
    show(1);
    show(3.14);
    show("hello world");
    return 0;
}