//运算符重载:使对象运算表现的和编译器内置类型一样
//编译器做对象运算的时候，会调用运算符重载函数，优先调用成员方法，如果没有成员方法，就在全局作用域内寻找合适的运算符重载函数
//单目运算符重载    operator++()//前置++    operator++(int)//后置++
// #include <cstring>
// #include<iostream>
// #include<string>
// using namespace std;
// class String
// {
// public:
//     String(const char * p=nullptr)
//     {
//         if (p!=nullptr)
//         {
//             pstr_=new char[strlen(p)+1];
//             strcpy(pstr_, p);
//         }else
//         {
//             pstr_=new char[1];
//             *pstr_='\0';
//         }
//     }
//     ~String()
//     {
//         delete [] pstr_;
//         pstr_=nullptr;
//     }
//     String(const String &str)
//     {
//         pstr_=new char[strlen(str.pstr_)+1];
//         strcpy(pstr_, str.pstr_);
//     }
//     String & operator=(const String &str)
//     {
//         if (this==&str)
//             return *this;
//         delete [] pstr_;
//         pstr_=new char[strlen(str.pstr_)+1];
//         strcpy(pstr_, str.pstr_);
//         return *this;
//     }
//     //给字符串类型提供迭代器
//     class iterator
//     {
//     public:
//     private:
//         char * p_;
//     };
//     //begin返回容器底层元素的迭代器的标志
//     iterator begin(){return iterator(pstr_);    }
//     iterator end();
// private:
//     char*pstr_;
//     friend ostream& operator<<(ostream &out,const String &str);
//     friend String operator+(const String &str1, const String &str2);
// };
// String operator+(const String &str1, const String &str2)
// {
//     char *ptmp=new char[strlen(str1.pstr_)+strlen(str2.pstr_)+1];
//     strcpy(ptmp, str1.pstr_);
//     strcat(ptmp, str2.pstr_);
//     return String(ptmp);
// }
// ostream& operator<<(ostream &out,const String &str)
// {
//     out<<str.pstr_<<endl;
// }
// //泛型算法参数接受的都是迭代器
// int main()
// {
//     string str1="hello world";//str1
//     //容器的迭代器
//     string::iterator it=str1.begin();//返回首元素的地址
//     for (;it!=str1.end();it++)
//     {
//         cout<<*it<<endl;//访问容器内部元素的值
//     }
//     // String str1;
//     // String str2="aaa";
//     // String str3="bbb";
//     // String str4=str3+str2;
//     // cout<<str4;
//     // return 0;
// }


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



//标准容器
//顺序容器
    // vector   deque   list
//容器适配置器
    //stack     queue    priority_queue
//关联容器
    //链式哈希表 增删查O（1）复杂度
    //无序容器
        //undorderd_set     unordered_multiset      unordered_map       unordered_multimap
    //有序容器 （红黑树）
        //set   multiset    map     multimap

//近容器
    //数组     string     bitset
//迭代器
    //iterator const_iterator
    //reserse_iterator和const_reverse_iterator
//函数对象(类似C的函数指针)
//greater,less

//泛型算法
//sort,find,find_if,binary_search,for_each






//vector
#include<iostream>
#include<vector>
using namespace std;
//vector向量容器
//底层的数据结构，动态开辟数组，每次以原来空间的二倍大小进行扩容
//vector<int>vec
//增加
//容器中，对象的构造析构，内存的开辟释放 通过谁来实现？
//容器的空间配置器 allocator  allocate deallocate construct destory

//vec.push_back(20);    末尾添加元素O(1) 导致容器空间扩容
//vec.insert(it,20)     在指定位置（it）添加元素20 O(n) 导致容器扩容

//删除
//vec.pop_back();       在末尾删除元素O(1)
//vec.erase(it);        删除it迭代器指向的元素O(n)
//查询
//operator[]            下表的随机访问
//iterator              迭代器进行遍历
//find for_each
//注意 对容器进行连续的插入或者删除操作，一定要更新迭代器，否则第一册insert或者erase完成，迭代器失效

//常用方法介绍
//size()
//empty()
//reserve(20)         vector预留空间    只给容器开辟指定到大小的空间，并不会添加新元素
//resize（20）        重置大小（扩容使用的） 不仅会给容器开辟空间还会给容器添加新元素
//swap               两个容器进行交换


//queue 双端队列容器
//底层数据结构：动态开辟的二维数组，一维数组从2开始，以2倍的方式进行扩容，原来的二维数组，从新的以为数组下标
//oldsize/2开始存放上下预留相同的空行，方便支持deque的首尾元素添加
//deque<int>deq
//增加
//deq.push_back(20) 从末尾开始添加O(1)
//deq.front(20)     从首部添加元素O（1）
//deq.insert(it,0) 指定位置添加元素

//删除
//deq.pop_back()    从末尾删除元素O(1)
//deq.pop_front()   从首部删除元素O(1)
//deq.erase()       从中间指定位置删除

//vector特点：动态数组，内存是连续的2倍的方式进行扩容，vector<int>vec;0-1-2-4-7...
//deque特点：动态开辟二维数组空间，第二维是固定长度的数组空间，扩容的时候（第一维数组进行二倍扩容）
            //deque底层数组空间是不是连续的 并不是（每一个第二维是连续的）
//vector和deque之间的区别
    //底层数据结构:前后都需要添加数据（数组）
    //前中后插入删除元素的时间复杂度：中间末尾o(1)前dequeO(1) vectorO(n)
    //对于内存的使用效率：vector需要空间是连续的，deque可以分块对数据进行存储，不需要内存空间是连续的
    //在中间进行insert或者erase，vector和deque谁的效率好一点？     vector更好一些
        //deque的第二维空间不是连续的所以在中间插入删除元素就要移动多个元素

//vector和list之间的区别
//底层数据结构:数组    双向链表



//容器适配器（设计模式适配器）
//适配器没有自己的数据结构，他是另外一容器的封装，他的方法全部是由底层依赖的容器进行实现的
//没有实现自己的迭代器
//stack                         push入栈 pop出栈 empty判断栈空 size返回元素个数   top查看栈顶元素
//stack依赖deque 为什么不依赖vector?
//1.vector初始内存使用效率太低了 没有deque好
//2.对于queue需要尾部插入头部删除 如果queue依赖vector其出队效率极低
//3.当存储大量数据时deque对于内存的利用率更大更好

//queue                         push入队 pop出队 front查看队头元素 back查看队尾元素 empty判断为空 size队大小
//queue依赖deque 为什么不依赖vector
//1.vector初始内存使用效率太低了 没有deque好
//2.对于queue需要尾部插入头部删除 如果queue依赖vector其出队效率极低
//3.当存储大量数据时deque对于内存的利用率更大更好

//priority——queue               push入队 pop出队 top查看队顶元素 back查看队尾元素 empty判断为空 size队大小
//priority_queue依赖vector 为社么依赖vector
//1.默认底层十一个大根堆结构           （大跟堆和小根堆）在连续内存上分配的


//