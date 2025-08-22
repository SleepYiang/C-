#include<cstdio>
#include<cmath>
#include<array>

//使用constexpr可以避免出现未定义行为
constexpr int func()
{
    int i=10;
    return i;
}
int main()
{
    //未定义的编译行为
    //使用constexpr来防止围挡一错误
    //左移右移
    //数组越界
    //指针加一个变量
    //设置大小端
    //加法溢出（无符号的溢出是定义行为）
    constexpr int i=func();
    printf("%d\n",i);
    return 0;
}