#include<iostream>
#include<vector>
#include<functional>
#include<algorithm>
using namespace std;
/*
    版的定期和函数对象
    1.C++ STL中的绑定器
    bind1st: operator()的第一个形参变量绑定成一个确定的值
    bins2nd: operator()的第二个形参变量绑定成一个确定的值

    2.bind（绑定器）和function（函数对象）机制


    3.lambada表达式
 */
template<typename Container>
void showConrainer(Container&con)
{
     typename Container::iterator it=con.begin();
    for (;it!=con.end();it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
}
int main()
{
    vector<int>vec;
    srand(time(nullptr));
    for (int i=0;i<20;++i)
    {
        vec.push_back(rand()%100+1);
    }
    showConrainer(vec);
    sort(vec.begin(),vec.end());//默认小到大排序
    showConrainer(vec);

    //greater是函数对象 二元函数对象
    sort(vec.begin(),vec.end(),greater<int>());//大到小排序
    showConrainer(vec);

    //把70按顺序插入到vec容器中，找到第一个小于70的数字
    //operator()(const T &val)
    //greater a>b
    //less    a<b
    //绑定器+二元函数对象-》一元函数对象
    //bind1st: + greater bool operator()(70,const_Ty&_Right)
    //bind2nd: + less bool operator()(const_Ty&_Left,70)
    auto it1=find_if(vec.begin(),vec.end(),
        bind1st(greater<int>(),70));
    if (it1!=vec.end())
    {
        vec.insert(it1,70);
    }
    showConrainer(vec);
    return 0;
}
//绑定器的底层实现
  //使用二元函数对象想。通过封装实现绑定的相关实现
//绑定器本身还是函数对象