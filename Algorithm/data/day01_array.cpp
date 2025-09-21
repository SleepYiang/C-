#include<iostream>
#include<stdio.h>
#include<time.h>
//数组的实现
class Array
{
public:
    Array(int size=10):mCur_(0),mCap_(size)
    {
        mpArr_ = new int[mCap_]();
    }
    ~Array()
    {
        delete[] mpArr_;
        mpArr_ = nullptr;
    }
    //末尾增加元素
    void push_back(int val)
    {
        if (mCur_==mCap_)
        {
            expand(2*mCap_);

        }
        mpArr_[mCur_++] = val;
    }
    //末尾删除元素
    void pop_back()
    {
        if (mCur_==0)
        {
            return;
        }
        mCur_--;
    }
    //按位置增加元素
    void insert(int pos,int val)
    {
        if (pos>mCur_||pos<0)
        {
            return;
        }
        if (pos==mCur_)
        {
            expand(2*mCur_);
        }
        //移动元素
        for (int i=mCur_-1;i>pos;i--)
        {
            mpArr_[i+1] = mpArr_[i];
        }
        mpArr_[pos] = val;
        mCur_++;
    }
    //按位置删除、
    void erase(int pos)
    {
        if (pos>mCur_||pos<0)
        {
            return;
        }
        //移动元素
        for (int i=mCur_+1;i>pos;i++)
        {
            mpArr_[i-1] = mpArr_[i];
        }
        mCur_--;
    }
    //查询元素
    int find(int val)
    {
        for (int i=0;i<mCur_;i++)
        {
            if (mpArr_[i]==val)
            {
                return i;
            }
        }
        return -1;
    }
    //打印数据
    void show()
    {
        for (int i=0;i<mCur_;i++)
        {
            printf("%d ",mpArr_[i]);
        }
        printf("\n");
    }
private:
    int *mpArr_;//至西安可扩容的数组内存
    int mCap_;//数组容量
    int mCur_;//数组有效元素的个数

    //内部数组扩容接口
    void expand(int size)
    {
        int* p=new int[size];
        memcpy(p,mpArr_,sizeof(int)*mCap_);
        delete[] mpArr_;
        mpArr_ = p;
        mCap_ = size;
    }
};
int main()
{
    Array arr;
    for (int i=0;i<10;i++)
    {
        arr.push_back(rand()%100);
    }
    arr.show();
    arr.pop_back();
    arr.show();
    arr.insert(10,200);
    arr.show();
    return 0;
}