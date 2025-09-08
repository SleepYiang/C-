/*
muduo网络库非用户提供了两个主要的网络库
TcpServer 用于编写服务器程序
TcpClient 用于编写客户端程序

epoll+线程池
好处 能够把网络I/O的代码和业务代码区分开
                        用户的来链接和断开  用户的可读写事件
 */
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<functional>
#include<iostream>
#include<string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace std::placeholders;
//基于muduo网络库开发服务器程序
//1.组合TcpServer对象
//2.创建EventLoop事件循环对象的指针
//3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
//4.在当服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
//5.设置合适的服务端线程数量,muduo库惠子分配I/O线程和worker线程
class ChatServer
{
public:
    ChatServer(EventLoop*loop,//事件循环
        const InetAddress&listenAddr,//IP+Port
        const string&nameArg)   //服务器的名字
    :server_(loop,listenAddr,nameArg),loop_(loop)
    {
        //给服务器注册用户来连接的创建和断开回调
        server_.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
        //给服务器注册用户读写事件回调
        server_.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));
        //设置服务器的线程数量 1个i/o线程 3个woker线程
        server_.setThreadNum(4);
    }

    //开启事件循环
    void start()
    {
        server_.start();
    }
private:
    //专门处理用户的连接创建和断开
    void onConnection(const TcpConnectionPtr& conn)
    {
        if (conn->connected())
        {
          cout<<conn->peerAddress().toIpPort()<<"->"
            <<conn->localAddress().toIpPort()<<"state:online"<<endl;
        }
        else
        {
            cout<<conn->peerAddress().toIpPort()<<"->"
             <<conn->localAddress().toIpPort()<<"state:offline"<<endl;
            conn->shutdown();//close(fd)
            //loop_->exquit();
        }
    }
    void onMessage(const TcpConnectionPtr&conn,//连接
        Buffer*buffer,//缓冲区
        Timestamp time)//接收到数据的事件信息
    {
        string buf=buffer->retrieveAllAsString();
        cout<<"recv data"<<buf<<"time"<<time.toString()<<endl;
        conn->send(buf);
    }
    TcpServer server_;//#1
    EventLoop *loop_;//#2 epoll
};
int main()
{
    EventLoop loop;
    InetAddress addr("192.168.244.128",6000);
    ChatServer server(&loop,addr,"ChatServer");

    server.start();
    loop.loop();//epoll_wait以阻塞的当时等待新用户的连接，以连接用户的读写事件等
    return 0;
}