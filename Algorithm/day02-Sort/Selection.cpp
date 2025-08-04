//选择排序
#include<iostream>
#include<vector>
using namespace std;
void SelectionSort(std::vector<int>&number)
{
    int n=number.size();
    //外层循环
    for (int i=0;i<n-1;i++)
    {
        //创建最小的数字
        int min=i;
        //内层循环
        for (int j=i+1;j<n;j++)
        {
            if (number[j]<number[min])
                min=j;
        }
        swap(number[min],number[i]);
    }
}
int main()
{
    vector<int>number={1,3,4,5,6,76,45,32,12,10};
    cout<<"Before Sorting:"<<endl;
    for (int i=0;i<number.size();i++)
    {
        cout<<number[i]<<" ";
    }
    cout<<endl;
    SelectionSort(number);
    cout<<"SelectionSort List:"<<endl;
    for (int i=0;i<number.size();i++)
    {
        cout<<number[i]<<" ";
    }
    return 0;
}
//选择排序的相关总结
    //选择排序是选择容器中的最小的数字见鬼晓得数字排列在容器的最前面从而实现选择排序