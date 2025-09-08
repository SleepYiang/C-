#include<sys/select.h>
#include<sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
int main()
{
    //创建监听socket
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    //绑定socket
    std::string ip="127.0.0.1";
    int port = 8080;
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr(ip.c_str());
    server.sin_port=htons(port);
    bind(lfd,(struct sockaddr*)&server,sizeof(server));
    //监听
    listen(lfd,128);
    //将监听事件给内核
    int maxfd=lfd;
    //初始化检测读集合
    fd_set rdset;
    fd_set rdtemp;
    //清零
    FD_ZERO(&rdset);
    //将监听的lfd设置到检测的读集合中
    FD_SET(lfd,&rdset);
    //通过select委托内核检测集合中文件描述符的状态，检测read缓冲区有没有数据
    //如果没有数据select接触阻塞返回
    //应该让内核持续检测
    while (1)
    {
        rdtemp=rdset;
        int num=select(maxfd+1,&rdtemp,NULL,NULL,NULL);
        if (FD_ISSET(lfd,&rdset))
        {
            struct sockaddr_in client;
            int clientlen=sizeof(client);
            int cfd= accept(lfd,(struct sockaddr*)&client,&clientlen);
            // (struct sockaddr*)&cliaddr, &cliLen
            FD_SET(cfd,&rdset);
            maxfd=cfd>maxfd?cfd:maxfd;
        }
        //没有新的通信连接
        for(int i=0; i<maxfd+1; ++i)
        {
            // 判断从监听的文件描述符之后到maxfd这个范围内的文件描述符是否读缓冲区有数据
            if(i != lfd && FD_ISSET(i, &rdtemp))
            {
                // 接收数据
                char buf[10] = {0};
                // 一次只能接收10个字节, 客户端一次发送100个字节
                // 一次是接收不完的, 文件描述符对应的读缓冲区中还有数据
                // 下一轮select检测的时候, 内核还会标记这个文件描述符缓冲区有数据 -> 再读一次
                // 	循环会一直持续, 知道缓冲区数据被读完位置
                int len = read(i, buf, sizeof(buf));
                if(len == 0)
                {
                    printf("客户端关闭了连接...\n");
                    // 将检测的文件描述符从读集合中删除
                    FD_CLR(i, &rdset);
                    close(i);
                }
                else if(len > 0)
                {
                    // 收到了数据
                    // 发送数据
                    write(i, buf, strlen(buf)+1);
                }
                else
                {
                    // 异常
                    perror("read");
                }
            }
        }
    }

    return 0;
}
