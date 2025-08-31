//C++内容总结
#include<iostream>
using namespace std;
/*
 * 一，关键字和语法
 * auto 可以更具右值，推导出左边的变量的类型，然后左边变量的类型也就已知了
 * nullptr 给指针专用（能够和整数进行区别）
 * foreach   for(Type val:container)  底层就是通过指针或者迭代器来实现的
 * 右值引用： 没有名字 没有内存  move移动语义foward类型完美转发
 *      模板型特性模板折叠（可变惨类型参数）
 *二，绑定器和函数对象
 *  function 函数对象
 *  bind    绑定器
 *  lambda  表达式
 *
 *  三，智能指针
 *  shared_ptr和weak_ptr
 *
 *  四，容器
 *  undered_set和undered_map
 *
 */