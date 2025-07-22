#ifndef SOCKET_H
#define SOCKET_H
#include<string>
#include<cstring>
#include<netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include<sys/socket.h>
namespace Adortheall
{
    namespace Socket
    {
        class Socket
        {
        public:
            Socket();
            Socket(int sockfd);
            ~Socket();
            bool bind(const std::string& ip,int port);
            bool listen(int backlog);
            bool connect(const std::string& ip,int port);
            int accept();
            int send(const char* burf,int len);
            int recv(char* buf,int len);
            void close();
        protected:
            std::string m_ip;
            int m_port;
            int m_sockfd;
        };
    }
}
#endif //SOCKET_H
