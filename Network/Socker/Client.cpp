#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

int main()
{

    // 第1步：创建客户端的socket。
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd==-1)
    {
        perror("socket"); return -1;
    }
    // 第2步：向服务器发起连接请求。
    int port=8080;
    string ip="127.0.0.1";
    struct sockaddr_in servaddr;               // 用于存放协议、端口和IP地址的结构体。
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;             // ①协议族，固定填AF_INET。
    servaddr.sin_port = htons(port);            // ②指定服务端的通信端口。
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    //servaddr.sin_addr.s_addr=inet_addr(argv[1]); // ③指定服务端的IP，只能用IP，不能用域名和主机名。
    if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)  // 向服务端发起连接清求。
    {
        perror("connect"); close(sockfd); return -1;
    }

    // 第3步：与服务端通讯，客户发送一个请求报文后等待服务端的回复，收到回复后，再发下一个请求报文。
    char buffer[1024];
    for (int ii=0;ii<10;ii++)  // 循环3次，将与服务端进行三次通讯。
    {
        int iret;
        memset(buffer,0,sizeof(buffer));
        sprintf(buffer,"这是第%d个超级女生，编号%03d。",ii+1,ii+1);  // 生成请求报文内容。
        // 向服务端发送请求报文。
        if ( (iret=send(sockfd,buffer,strlen(buffer),0))<=0)
        {
            perror("send"); break;
        }
        cout << "发送：" << buffer << endl;

        memset(buffer,0,sizeof(buffer));
        // 接收服务端的回应报文，如果服务端没有发送回应报文，recv()函数将阻塞等待。
        if ( (iret=recv(sockfd,buffer,sizeof(buffer),0))<=0)
        {
            cout << "iret=" << iret << endl; break;
        }
        cout << "接收：" << buffer << endl;

        sleep(1);
    }

    // 第4步：关闭socket，释放资源。
    close(sockfd);
}

