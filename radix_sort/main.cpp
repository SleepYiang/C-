#include<fmt/format.h>
#include<algorithm>
#include<fmt/ranges.h>
#include<random>
#include<vector>
//基数排序
void radix_sor(uint32_t *a,size_t n)
{
    //基于比较的排序   merge_sort,quick_sort，insert_sort   O（nlog n）
    //bin_sort,radix_sort:是一个整数（有上限 ）类型,O(n)
}
int main()
{
    //生成100个0~10000以内的随机数
    std::vector<uint32_t> a(100);
    std::generate(a.begin(), a.end(), [rng=std::mt19937{},uni=std::uniform_int_distribution<uint32_t>
        {0,10000}]()mutable
    {
        return uni(rng);
    });
    //稳定排序 stable_sort 4a 4b 1 2 =1 2 4a 4b (稳定排序不会改变相同数字的顺序)
    //不稳定排序 unstable_sort 4a 4b 1 2 = 1 2 4b 4a (不稳定会让数字顺序发生改变)



    //打印排序前的数组
    fmt::print("========before=========");
    fmt::print("{}",a);

    //排序
    std::sort(a.begin(), a.end());

    fmt::print("========after=========");
    fmt::print("{}",a);
}