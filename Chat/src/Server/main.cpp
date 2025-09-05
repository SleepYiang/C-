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
}