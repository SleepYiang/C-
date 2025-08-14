#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
int main()
{
    //初始化来凝结的socket
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
    //监听socket
    listen(sockfd,1024);

    //初始化用于监听的listensock
    int listensock=sockfd;
    printf("listensock=%d\n",listensock);
    if (listensock<0)
    {
        printf("initserver() failed");
        return -1;
    }

    //fds存放需要监视的socket
    pollfd fds[1024];

    //初始化数组，把全部的socket设置为-1，如果数组中的socket的值为-1，那么，poll将忽略他
    for (int ii=0;ii<1024;ii++)
    {
        fds[ii].fd=-1;
        //打算让poll监视listensock读事件
        fds[listensock].fd=listensock;
        fds[listensock].events=POLLIN;   //POLLIN表示读事件，POLLOUT表示写事件
        int maxfd=listensock;    //fds数组中监视的socket实际大小
        while (true)
        {
            //调用poll（）等待事件的发生(监视那些socket发生了事件)
            int infds=poll(fds,maxfd+1,10000); //超时时间为10秒
            //如果infds<0，表示poll()失败
            if (infds<0)
            {
                perror("poll");
                break;
            }
            if (infds==0)
            {
                printf("poll() timeout");
                continue;
            }
            //如果infds>0,表示有事件发生，infds存放了已经发生的事件个数
            for (int eventfd=0;eventfd<maxfd;eventfd++)
            {
                if (fds[eventfd].fd<0) continue;//如哦为负数忽略它

                if ((fds[eventfd].revents&POLLIN)==0) continue; //如果没有读事件，continue

                //如果事件发生的是listensocket，表示已连接的队列中已经准备好的socket(有新的客户端连接上来了)
                if (eventfd==listensock)
                {
                    struct sockaddr_in client;
                    socklen_t len=sizeof(client);
                    int clientsock=accept(listensock,(struct sockaddr*)&client,&len);
                    if (clientsock<0)
                    {
                        perror("accept");
                        continue;
                    }
                    printf("accept client(sockrt=%d)ok.\n",clientsock);
                    //修改fds数组中clientsock位置的元素
                    fds[eventfd].fd=clientsock;
                    fds[eventfd].events=POLLIN;
                    if (maxfd<clientsock)
                        maxfd=clientsock;  //更新maxfd的值
                }
                else
                {
                    // 如果是客户端连接的socke有事件，表示有报文发过来了或者连接已断开。
                    char buffer[1024]; // 存放从客户端读取的数据。
                    memset(buffer,0,sizeof(buffer));
                    if (recv(eventfd,buffer,sizeof(buffer),0)<=0)
                    {
                        // 如果客户端的连接已断开。
                        printf("client(eventfd=%d) disconnected.\n",eventfd);

                        close(eventfd);               // 关闭客户端的socket。
                        fds[eventfd].fd=-1;        // 修改fds数组中clientsock位置的元素，置为-1，poll将忽略该元素。

                        // 重新计算maxfd的值，注意，只有当eventfd==maxfd时才需要计算。
                        if (eventfd == maxfd)
                        {
                            for (int ii=maxfd;ii>0;ii--)  // 从后面往前找。
                            {
                                if (fds[ii].fd!=-1)
                                {
                                    maxfd = ii; break;
                                }
                            }
                        }
                    }
                    else
                    {
                        // 如果客户端有报文发过来。
                        printf("recv(eventfd=%d):%s\n",eventfd,buffer);

                        send(eventfd,buffer,strlen(buffer),0);
                    }

                }
            }
        }
        return 0;

}