#include<iostream>
// 将数组栈区
int arr[5];
int num[4] = {1, 2, 3, 4};
// 将数组存放在堆区(需要手动释放)
// int *nums1[4] = new int[4];
// int *arr[4] = new int[4]{1, 2, 3, 4};
//访问数组元素
int randomAccess(int *nums, int size)
{
    // 在区间 [0, size) 中随机抽取一个数字
    int randomIndex = rand() % size;
    // 获取并返回随机元素
    int randomNum = nums[randomIndex];
    return randomNum;
}
//插入数组元素
void insert(int *nums, int size, int num, int index) {
    // 把索引 index 以及之后的所有元素向后移动一位
    for (int i = size - 1; i > index; i--) {
        nums[i] = nums[i - 1];
    }
    // 将 num 赋给 index 处的元素
    nums[index] = num;
}
//删除相关的元素
void remove(int *nums, int size, int index) {
    // 把索引 index 之后的所有元素向前移动一位
    for (int i = index; i < size - 1; i++) {
        nums[i] = nums[i + 1];
    }
}
//遍历所有元素
void traverse(int *nums, int size) {
    int count = 0;
    // 通过索引遍历数组
    for (int i = 0; i < size; i++) {
        count += nums[i];
    }
}
//查找相关元素
int find(int *nums, int size, int target) {
    for (int i = 0; i < size; i++) {
        if (nums[i] == target)
            return i;
    }
    return -1;
}

int main()
{
    //水一条

}