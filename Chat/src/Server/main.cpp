#include"chatserver.hpp"
#include<iostream>

using namespace std;
int main()
{
    EventLoop loop;
    InetAddress addr("192.168.244.128",6000);
    ChatServer server(&loop,addr,"ChatServer");
    server.start();
    loop.loop();
    return 0;
    //{"msgid":1,"id":13,"password":"123456"}
    //{"msgid":3,"name":"zhang san yi","password":"123456"}
}


