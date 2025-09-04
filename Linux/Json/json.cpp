//让数据序列化 让消息转化为字节流/字符流 (Json流)
//数据序列化和反序列化
//json一个通用的转换工具（中间转换）是的不同语言封装的代码可以相互通信拥有一个统一的格式
#include"json.hpp"
using json=nlohmann::json;//简化json的名字
#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;
//json序列化的示例1   (数据结构很像哈希map)
void func1()
{
    json js;
    js["msg_type"]=2;
    js["from"]="zhang san";
    js["to"]="li si";
    js["msg"]="hello what are you doing now?";

    string sendBuf=js.dump();
    cout<<sendBuf.c_str()<<endl;
}
//json实例化更复杂的
void func2()
{
    json js;
    //添加数组
    js["id"]={1,2,3,4,5,8,6,7};
    //添加key_value
    js["name"]="zhang san";
    //添加对象
    js["msg"]["zhang san"]="hello world";
    js["msg"]["liu shuo"]="hello china";
    //上面等同于下面这句一次性添加数组对象
    js["msg"]={{"zhang san","hello world"},{"liu shuo","hello china"}};
    cout<<js<<endl;
}
int main()
{
    func1();

    func2();
    return 0;
}