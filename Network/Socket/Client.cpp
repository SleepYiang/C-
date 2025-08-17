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
    if(sockfd<0)
    {
        printf("socket creat filed error:%d,errmsg:%s \n",errno,strerror(errno));
        return 1;
    }
    else
    {
        printf("socket creat success\n");
    }
    //连接服务端
    std::string ip="127.0.0.1";
    int port=8080;
    struct sockaddr_in client;
    std::memset(&client,0,sizeof(client));
    client.sin_family=AF_INET;
    client.sin_port=htons(port);
    client.sin_addr.s_addr=inet_addr(ip.c_str());
    if (connect(sockfd,(struct sockaddr*)&client,sizeof(client))<0)
    {
        printf("connect failed error:%d,errmsg:%s",errno,strerror(errno));
        return 1;
    }
    //向服务端发送数据
    std::string data="hello,world";
    ::send(sockfd,data.c_str(),data.size(),0);
    char recvbuf[1024]={0};
    recv(sockfd,recvbuf,sizeof(recvbuf),0);
    std::cout<<"recv: "<<recvbuf<<std::endl;
    close(sockfd);
    return 0;
}