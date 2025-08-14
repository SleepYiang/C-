#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<string>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
int main()
{
    //创建socket
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        perror("socket");
    }
    //绑定socket
    std::string ip="127.0.0.1";
    int port=8080;
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr.s_addr=inet_addr(ip.c_str());
    bind(sockfd,(struct sockaddr*)&server,sizeof(server));
    //监听socke
    listen(sockfd,1024);

    //初始化用监听的listnsock
    int listensock=sockfd;
    printf("listensock=%d\n",listensock);
    if (listensock < 0)
    {
        printf("initserver() failed.\n"); return -1;
    }

    //创建epoll句柄
    int epollfd=epoll_create(1);
    //为服务端的listensock准备读事件
    epoll_event ev;
    ev.data.fd=listensock;
    ev.events=EPOLLIN;       //让epoll监听listensock的读事件
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listensock,&ev);     // 把需要监视的socket和事件加入epollfd中。

    epoll_event evs[10];      // 存放epoll返回的事件。

    while (true)        // 事件循环。
    {
        // 等待监视的socket有事件发生。
        int infds=epoll_wait(epollfd,evs,10,-1);

        // 返回失败。
        if (infds < 0)
        {
            perror("epoll() failed"); break;
        }

        // 超时。
        if (infds == 0)
        {
            printf("epoll() timeout.\n"); continue;
        }

        // 如果infds>0，表示有事件发生的socket的数量。
        for (int ii=0;ii<infds;ii++)       // 遍历epoll返回的数组evs。
        {
            // printf("ptr=%s,events=%d\n",evs[ii].data.ptr,evs[ii].events);

            // 如果发生事件的是listensock，表示有新的客户端连上来。
            if (evs[ii].data.fd==listensock)
            {
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int clientsock = accept(listensock,(struct sockaddr*)&client,&len);

                printf ("accept client(socket=%d) ok.\n",clientsock);

                // 为新客户端准备读事件，并添加到epoll中。
                ev.data.fd=clientsock;
                ev.events=EPOLLIN;
                epoll_ctl(epollfd,EPOLL_CTL_ADD,clientsock,&ev);
            }
            else
            {
                // 如果是客户端连接的socke有事件，表示有报文发过来或者连接已断开。
                char buffer[1024]; // 存放从客户端读取的数据。
                memset(buffer,0,sizeof(buffer));
                if (recv(evs[ii].data.fd,buffer,sizeof(buffer),0)<=0)
                {
                    // 如果客户端的连接已断开。
                    printf("client(eventfd=%d) disconnected.\n",evs[ii].data.fd);
                    close(evs[ii].data.fd);            // 关闭客户端的socket
                    // 从epollfd中删除客户端的socket，如果socket被关闭了，会自动从epollfd中删除，所以，以下代码不必启用。
                    // epoll_ctl(epollfd,EPOLL_CTL_DEL,evs[ii].data.fd,0);
                }
                else
                {
                    // 如果客户端有报文发过来。
                    printf("recv(eventfd=%d):%s\n",evs[ii].data.fd,buffer);

                    // 把接收到的报文内容原封不动的发回去。
                    send(evs[ii].data.fd,buffer,strlen(buffer),0);
                }
            }
        }
    }
  return 0;
}