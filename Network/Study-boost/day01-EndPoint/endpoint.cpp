#include"endpoint.h"
#include <iostream>
#include<boost/asio.hpp>
#include<string>
using namespace boost;
int client_end_point()
{
    std::string raw_ip_address="127.4.8.1";
    unsigned short port_num=3333;
    //创建错误关键字
    boost::system::error_code ec;
    asio::ip::address ip_address=asio::ip::address::from_string(raw_ip_address,ec);
    //判断地址转换是否成功
    if (ec.value()!=0)
    {
        std::cout<<"Failed to parse IP address. Error code="<<ec.value()<<".Message is"<<ec.message();
        return ec.value();
    }
    //将ip地址端口号转换成端点依靠端点进行通信
    asio::ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}


//服务器端
int server_end_point()
{
    unsigned short port_num=3333;
    asio::ip::address ip_address=asio::ip::address_v4::any();
    asio::ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

//拆功能键socket
int create_tcp_socket()
{
    //创建上下文
    asio::io_context ioc;
    //创建协议
    asio::ip::tcp protocol=asio::ip::tcp::v4();
    asio::ip::tcp::socket sock(ioc);
    boost::system::error_code ec;
    sock.open(protocol,ec);
    if (ec.value()!=0)
    {
        std::cout<<"Failed to parse IP address. Error code="
        <<ec.value()<<".Message is"<<ec.message();
        return ec.value();
    }
    return 0;
}

//创建能键服务器
int create_acceptor_socket()
{
    asio::io_context ios;
    //新版boost的建立方式
    asio::ip::tcp::acceptor a(ios,asio::ip::tcp::endpoint
        (asio::ip::tcp::v4(),server_end_point()));
    return 0;
}
//绑定服务器
int bind_acceptor_socket()
{
    unsigned short port_num=3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),port_num);
    asio::io_context ios;
    asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
    boost::system::error_code ec;
    acceptor.bind(ep,ec);
    if (ec.value()!=0)
    {
        std::cout<<"Failed to parse IP address. Error code="
        <<ec.value()<<".Message is"<<ec.message();
        return ec.value();
    }
    return 0;
}
int connect_to_end()
{
    std::string raw_ip_address="192.168.1.124";
    unsigned short port_num=3333;
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),port_num);
        asio::io_context ios;
        asio::ip::tcp::socket sock(ios,ep.protocol());
        sock.connect(ep);
    }catch (system::system_error& e)
    {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();

        return e.code().value();
    }
}
//通过域名解析实现链接
int dns_connect_to_end()
{
    std::string host="llfc.club";
    std::string port_num=3333;
    asio::io_context ios;
    //创建dns解析器
    asio::ip::tcp::resolver::query resolver_query(host,port_num,asio::ip::tcp::resolver::query::numeric_service);
    asio::ip::tcp::resolver resolver(ios);
    try
    {
        asio::ip::tcp::resolver::iterator it=resolver.resolve(resolver_query);
        asio::ip::tcp::socket sock(ios);
        asio::connect(sock,it);
    }catch (system::system_error& e)
    {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
        return e.code().value();
    }
}
//服务器接受连接
int accept_new_connection()
{
    const int BACKLOG_SIZE=30;
    unsigned short port_num=3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),port_num);
    asio::io_context ios;
    try
    {
        asio::ip::tcp::acceptor acceptor(ioa,ep.protocol());
        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);
        asio::ip::tcp::socket sock(ios);
        acceptor.accept(sock);
    }catch (system::system_error& e)
    {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();

        return e.code().value();
    }
}
