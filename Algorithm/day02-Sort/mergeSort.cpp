#include<iostream>
#include<vector>
void merge(std::vector<int>&nums,int left,int mid,int right)
{
    std::vector<int> temp(right-left+1);
    int i=left,j=mid+1,k=0;
    while (i<=mid && j<=right)
    {
        if (nums[i]<=nums[j])
        {
            temp[k++]=nums[i++];
        }
        else
        {
            temp[k++]=nums[j++];
        }
    }
    while (i<=mid)
    {
        temp[k++]=nums[i++];
    }
    while (j<=right)
    {
        temp[k++]=nums[j++];
    }
    for (int k=0;k<temp.size();k++)
    {
        nums[left+k]=temp[k];
    }
}
void mergeSort(std::vector<int>&nums,int left,int right)
{
    if (left>=right)
        return;
    int mid = left + ((right - left) / 2);
    mergeSort(nums,left,mid);
    mergeSort(nums,mid+1,right);
    merge(nums,left,mid,right);
}
int main()
{
    std::vector<int>number={12,23,34,45,56,67,78,89,90,233,122,3,4,34,523,};
    std::cout<<"before sorting"<<std::endl;
    for(int i=0;i<number.size();i++)
    {
        std::cout<<number[i]<<" ";
    }
    std::cout<<std::endl;
    mergeSort(number,0,number.size()-1);
    std::cout<<"after sorting"<<std::endl;
    for(int i=0;i<number.size();i++)
    {
        std::cout<<number[i]<<" ";
    }
    std::cout<<std::endl;
    return 0;
}