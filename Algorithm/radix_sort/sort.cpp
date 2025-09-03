#include<fmt/format.h>
#include<algorithm>
#include<fmt/ranges.h>
#include<random>
#include<vector>

int main()
{
    //生成100个0~10000以内的随机数
    std::vector<uint32_t> a(100);
    std::generate(a.begin(), a.end(), [rng=std::mt19937{},uni=std::uniform_int_distribution<uint32_t>
        {0,10000}]()mutable
    {
        return uni(rng);
    });

    //打印排序前的数组
    fmt::print("========before=========");
    fmt::print("{}",a);

    //排序
    std::sort(a.begin(), a.end());

    fmt::print("========after=========");
    fmt::print("{}",a);
}