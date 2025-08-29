#include<iostream>
#include<memory>
using namespace std;
template<typename T>
class CSmartPtr
{
public:
    CSmartPtr(T *ptr=nullptr):mptr(ptr){}
    ~CSmartPtr(){delete mptr;}
    T&operator*(){return *mptr;}
    T* operator->(){return mptr;}
private:
    T*mptr;
};
int main()
{
    //不带引用计数的智能指针(怎么解决浅拷贝的问题)
    //auto_ptr: c++库中带的
    //c++新标准
    //scoped_ptr
    //unique_ptr
    CSmartPtr<int> p1(new int);
    //p1, p2=>new int
    CSmartPtr<int> p2(p1);

    // auto_ptr<int>prt1(new int);
    // auto_ptr<int>prt2(ptr1);
    // *ptr2=20;
    // cout<<*ptr1<<endl;

    //不推荐使用auto_ptr，vector<auto_ptr<int>> vec1; vec2(vec1);
    //auto_ptr调用拷贝构造的时候会将原有指针的所有权转移

    //scoped_ptr(const scoped_ptr<T>&)=delete;
    //scoped_ptr<T>&operator=(const scoped_ptr<T>&)=delete
    //在scoped_ptr中的拷贝赋值，拷贝构造是被禁止的

    //推荐使用unique_ptr
    //unique_ptr(const unique_ptr<T>&)=delete;
    //unique_ptr<T>& operator=(const unique_ptr<T>&)=delete;

    //unique_ptr(unique_ptr<T>&&src);
    //unique_ptr<T>& operator=(unique_ptr<T>&&src);

    //template<typename T>
    //unique_ptr<T>getSmartPtr()
    //{
    //      unique_ptr<T> ptr(new T());
    //      return ptr;
    //}
    //
    //怎么解决浅拷贝
        //std::move c++中右值引用 std::move得到当前变量的右值类型
    unique_ptr<int> p3(new int);
    unique_ptr<int> p4(std::move(p3));
}