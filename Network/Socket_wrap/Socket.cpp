#include"Socket.h"
using namespace Adortheall::Socket;
Socket::Socket():m_ip(""),m_port(0),m_sockfd(0)
{
    //创建socket
    m_sockfd=::socket(AF_INET,SOCK_STREAM,0);
    if(m_sockfd<0)
    {
        printf("socket creation failed error:%d,errmsg:%s",errno,strerror(errno));
        return;
    }
}
Socket::Socket(int sockfd):m_sockfd(sockfd),m_ip(""),m_port(0)
{

}

Socket::~Socket()
{
    close();
}
bool Socket::bind(const std::string& ip,int port)
{
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family=AF_INET;
    if (ip.empty())
    {
        sockaddr.sin_addr.s_addr=htonl(INADDR_ANY);//任意地址都可以
    }else
    {
        sockaddr.sin_addr.s_addr=inet_addr(ip.c_str());
    }
    sockaddr.sin_port=htons(port);
    if (::bind(m_sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr)) < 0)
    {
        printf("bind failed error:%d,errmsg:%s",errno,strerror(errno));
        return false;
    }
    return true;
}
bool Socket::listen(int backlog)
{
    if (::listen(m_sockfd,1024) < 0)
    {
        printf("listen failed error:%d,errmsg:%s",errno,strerror(errno));
        return false;
    }else
    {
        return true;
    }
}
bool Socket::connect(const std::string& ip, int port)
{
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_port=htons(port);
    sockaddr.sin_addr.s_addr=inet_addr(ip.c_str());
    if (::connect(m_sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr)) < 0)
    {
        printf("connect failed error:%d,errmsg:%s",errno,strerror(errno));
        return false;
    }
    return true;
}
int Socket::accept()
{
    int connfd=::accept(m_sockfd,nullptr,nullptr);
    if (connfd<0)
    {
        printf("accept failed error:%d,errmsg:%s",errno,strerror(errno));
        return false;
    }
    return connfd;
}
int  Socket::send(const char* buf,int len)
{
    return ::send(m_sockfd,buf,len,0);
}
int Socket::recv( char* buf,int len)
{
    return ::recv(m_sockfd,buf,len,0);
}
void Socket::close()
{
    if (m_sockfd>0)
    {
        ::close(m_sockfd);
        m_sockfd=0;
    }
}