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
    return 0;

}