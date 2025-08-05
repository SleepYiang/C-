#include<iostream>
#include<vector>
void bubbleSort(std::vector<int>&number)
{
    int n=number.size();
    for (int i=0;i<n;i++)
    {
        for (int j=i+1;j<n;j++)
        {
            if (number[j]<number[i])
            {
                int temp=number[i];
                number[i]=number[j];
                number[j]=temp;
            }
        }
    }
}
int main()
{
    std::vector<int> number={13,24,23,14,35,56,27,18,49};
    std::cout<<"before sorting"<<std::endl;
    for(int i=0;i<number.size();i++)
    {
        std::cout<<number[i]<<" ";
    }
    std::cout<<std::endl;
    bubbleSort(number);
    std::cout<<"after sorting"<<std::endl;
    for(int i=0;i<number.size();i++)
    {
        std::cout<<number[i]<<" ";
    }
    return 0;
}
