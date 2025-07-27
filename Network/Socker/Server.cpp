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
  // 第1步：创建服务端的socket。
  int listenfd = socket(AF_INET,SOCK_STREAM,0);
  if (listenfd==-1)
  {
    perror("socket"); return -1;
  }

  // 第2步：把服务端用于通信的IP和端口绑定到socket上。
  int port=8080;
  string ip="127.0.0.1";
  struct sockaddr_in servaddr;                // 用于存放协议、端口和IP地址的结构体。
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family=AF_INET;                // ①协议族，固定填AF_INET。
  servaddr.sin_port=htons(port);     // ②指定服务端的通信端口。
  servaddr.sin_addr.s_addr=inet_addr(ip.c_str()); // ③如果操作系统有多个IP，全部的IP都可以用于通讯。
  //servaddr.sin_addr.s_addr=inet_addr("192.168.101.138"); // ③指定服务端用于通讯的IP(大端序)。
  // 绑定服务端的IP和端口。
  if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)
  {
    perror("bind"); close(listenfd); return -1;
  }

  // 第3步：把socket设置为可连接（监听）的状态。
  if (listen(listenfd,5) == -1 )
  {
    perror("listen"); close(listenfd); return -1;
  }

  // 第4步：受理客户端的连接请求，如果没有客户端连上来，accept()函数将阻塞等待。
  int clientfd=accept(listenfd,0,0);
  if (clientfd==-1)
  {
    perror("accept"); close(listenfd); return -1;
  }

  cout << "客户端已连接。\n";

  // 第5步：与客户端通信，接收客户端发过来的报文后，回复ok。
  char buffer[1024];
  while (true)
  {
    int iret;
    memset(buffer,0,sizeof(buffer));
    // 接收客户端的请求报文，如果客户端没有发送请求报文，recv()函数将阻塞等待。
    // 如果客户端已断开连接，recv()函数将返回0。
    if ( (iret=recv(clientfd,buffer,sizeof(buffer),0))<=0)
    {
       cout << "iret=" << iret << endl;  break;
    }
    cout << "接收：" << buffer << endl;

    strcpy(buffer,"ok");  // 生成回应报文内容。
    // 向客户端发送回应报文。
    if ( (iret=send(clientfd,buffer,strlen(buffer),0))<=0)
    {
      perror("send"); break;
    }
    cout << "发送：" << buffer << endl;
  }

  // 第6步：关闭socket，释放资源。
  close(listenfd);   // 关闭服务端用于监听的socket。
  close(clientfd);   // 关闭客户端连上来的socket。
}
