//
// Created by Adrotheall on 25-7-22.
//
#include"Socket.h"
using namespace Adortheall::Socket;
int main()
{
    //创建Socket
    Socket server;
    //绑定Socket
    std::string ip="127.0.0.1";
    int port=8080;
    server.bind("127.0.0.1",8080);
    //监听
    server.listen(1024);
    while(true)
    {
        int connfd=server.accept();
        Socket client(connfd);
        char buffer[1024];
        size_t size=client.recv(buffer,sizeof(buffer));
        printf("%s",buffer);

        client.send(buffer,size);
        client.close();
    }
    return 0;
}