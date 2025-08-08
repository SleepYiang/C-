//插入排序
//遍历所有的元素，找出一个基类元素，如果比基类元素大就在基类元素的右边，反之则亦然
#include<vector>
#include<iostream>
void insertionSort(std::vector<int>&number)
{
    for (int i=1;i<number.size();i++)
    {
        int temp=number[i];
        int j=i-1;
        while (j>0&&number[j]>temp)
        {
            number[j+1]=number[j];
            j--;
        }
        number[j+1]=temp;
    }
}
void display(std::vector<int>&number)
{
    for(int i=0;i<number.size();i++)
    {
        std::cout<<number[i]<<" ";
    }
    std::cout<<std::endl;
}
int main()
{
    std::vector<int> sort={2,3,4,2,3,234,2,14,5,323};
    std::cout<<"before insertion sort:"<<std::endl;
    display(sort);
    std::cout<<"after insertion sort:"<<std::endl;
    insertionSort(sort);
    display(sort);
    return 0;
}