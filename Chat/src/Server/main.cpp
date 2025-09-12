#include "chatserver.hpp"
#include"chatservice.hpp"
#include<iostream>
#include<csignal>
using namespace std;
//处理服务器ctrl+c结束后，重置user的状态信息
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}
int main()
{
    signal(SIGINT,resetHandler);

    EventLoop loop;
    InetAddress addr("192.168.244.128",6000);
    ChatServer server(&loop,addr,"ChatServer");

    server.start();
    loop.loop();

    //{"msgid":3}
    //{"msgid":2,"name":"zhang san yi","password":"123456"}
    //{"msgid":1,"id":22,"password":"123456"}
    //{"msgid":1,"id":22,"password":"123456"}{"msgid":1,"id":22,"password":"123456"}
    //{"msgid":1,"id":15,"password":"666666"}
    //{"msgid":5,"id":22,"from":"zhang san yi","to":15,"msg":"hello"}
    //{"msgid":5,"id":15,"from":"li si","to":22,"msg":"hello!"}
    //update user set state ='offline' where id=22
    //{"msgid":6,"id":22,"friendid":15}
    //std::cout <<"hello world";
    //
    //fgdsfgfgsdfg
    return 0;
//*********************************note**************************************
    //引入负载均衡的相关知识点（通过业务分发、传输层分发、网络层的分发、数据链路层的分发）{LVS（偏底层的负载均衡）}
    //负载均衡激素hi一个中间件采用不同的算法实现负载均衡（轮询  哈希 等算法实现）
        //1.把client的请求按照负载算法分发到具体的业务服务器上面
        //2.能够和chatserver保持心跳机制，检测chatserver故障（时刻检测网络的连接状态）
        //3.能够发现新添加的chatserver设备，方便扩展服务器数量

    //如何解决跨服务器通信的问题
        //中间件(为了减少服务器之间的耦合)基于发布-订阅的消息队列Redis（）
    //选择nginx的tcp组在均衡模块
        //如何进行nginx源码编译，包含tcp 负载均衡模块
        //2.nginc.conf废止文件中如何配置负载均衡
        //3.nginx的平滑加载配置文件启动

    //jiben biji

}