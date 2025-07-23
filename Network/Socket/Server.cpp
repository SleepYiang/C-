#include<cstring>
#include<netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include<sys/socket.h>
int main()
{
    //创建socket
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd<0)
    {
        printf("socket creation failed error:%d,errmsg:%s",errno,strerror(errno));
    }
    else
    {
        printf("socket successfully created \n");
    }
    //绑定scoket
    std::string ip="127.0.0.1";
    int port=8080;
    struct sockaddr_in server;
    std::memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr.s_addr=inet_addr(ip.c_str());
    if (bind(sockfd,(struct sockaddr*)&server,sizeof(server))<0)
    {
        printf("bind failed error:%d,errmsg:%s",errno,strerror(errno));
        return 1;
    }
    else
    {
        printf("bind successful \n");
    }

    //监听socket
    if (listen(sockfd,1024)<0)
    {
        printf("listen failed error:%d,errmsg:%s",errno,strerror(errno));
    }else
    {
        printf("listening... \n");
    }
    while (true)
    {
        //连接套接字
        int connfd=accept(sockfd,nullptr,nullptr);
        if (connfd<0)
        {
            printf("accept failed error:%d,errmsg:%s",errno,strerror(errno));
            return 1;
        }
        //创建buffer
        char buf[1024]={0};
        //接受客户端的数据
        recv(connfd,buf,sizeof(buf),0);
        std::cout<<"recv from server msg:%s"<<buf;
        //向客户端发送数据
        send(connfd,buf,sizeof(buf),0);
    }
    //关闭服务端
    close(sockfd);
    return 0;
}