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

//json实例化代码3
void func3()
{
    json js;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;
    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    cout<<js<<endl;//
}

string func4()
{
    json js;
    js["msg_type"]=2;
    js["from"]="zhang san";
    js["to"]="li si";
    js["msg"]="hello what are you doing now?";

    string sendBuf=js.dump();
    return sendBuf;
}

string func5()
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
    //cout<<js<<endl;
    return js.dump();
}

string func6()
{
    json js;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;
    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    return js.dump();
}
int main()
{
    //func1();

    //func2();

    //func3();

     string recBuf=func6();
    // //数据的反序列化   json字符串-》反序列化 数据对象（看作容器方便访问）
     json jsbuf=json::parse(recBuf);
    // cout<<jsbuf["msg_type"]<<endl;
    // cout<<jsbuf["from"]<<endl;
    // cout<<jsbuf["to"]<<endl;
    // cout<<jsbuf["msg"]<<endl;

    // cout<<jsbuf["id"]<<endl;
    // auto arr=jsbuf["id"];
    // cout<<arr[2]<<endl;
    // auto msgjs=jsbuf["msg"];
    // cout<<msgjs["zhang san"]<<endl;

    vector<int>vec=jsbuf["list"];//js对象里面的数组类型，直接放入vector容器当中
    for (int &v :vec)
    {
        cout<<v<<" ";
    }
    cout<<endl;

    map<int, string> mymap=jsbuf["path"];
    for (auto&p : mymap)
    {
        cout<<p.first<<" ";
        cout<<p.second<<" ";
    }
    cout<<endl;
    return 0;
}