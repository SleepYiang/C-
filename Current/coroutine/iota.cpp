#include<coroutine>
#include <ranges>
#include<iostream>
#include<co_context/generator.hpp>
using co_context::generator;
generator<int> iota()
{
    int n=0;
    while (true)
    {
        co_yield n;
        ++n;
    }
}
int main()
{
    using std::ranges::views::drop,std::views::take;
    using std::cout,std::endl;
    for (int x:iota())
    {
        cout<<x<<endl;
    }
    return 0;
}
//协程暂停后，相关的数据是被保存起来的（在内存中）