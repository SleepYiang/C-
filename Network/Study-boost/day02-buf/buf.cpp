#include<boost/asio.hpp>
#include<iostream>
#include<vector>
using namespace boost;
//繁琐版使用const buffer
void use_const_buffer()
{
    std::string buf="hello world";
    asio::const_buffer asio_buf(buf.c_str(),buf.length());
    std::vector<asio::const_buffer> buffer_sequence;
    buffer_sequence.push_back(asio_buf);
}

//简化版使用const_buffer
void use_buffer_str()
{
    asio::const_buffers_1 output_buf=asio::buffer("helloworld");

}

//使用数组缓存const_buffer
void use_buffer_array()
{
    const size_t BUF_SIZE_BYTES=20;
    std::unique_ptr<char[]>buf(new char[BUF_SIZE_BYTES]);
    auto input_buf=asio::buffer(static_cast<void*>(buf.get()),BUF_SIZE_BYTES);
}

//同步读写
void write_to_socket(asio::ip::tcp::socket&sock)
{
    std::string buf="helloworld";
    std::size_t total_bytes_written=0;
    //循环发送
    //write_some返回每次写入的字节数
    while (total_bytes_written!=buf.length())
    {
        total_bytes_written+=sock.write_some(asio::buffer(buf.c_str()+total_bytes_written,
            buf.length()-total_bytes_written));
    }
}
//相关实例(同步通过write_some 的方式实现)
int send_data_by_write_some()
{
    std::string raw_ip_address="127.0.0.1";
    unsigned short port=3333;
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),port);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc,ep.protocol());
        sock.connect(ep);
        write_to_socket(sock);
    }catch (system::system_error &e)
    {
        std::cout<<e.what()<<std::endl;
        return -1;
    }
    return 0;
}
//同步发送不采用轮询的方式完成所有信息的发送
int seng_data_by_send()
{
    std::string raw_ip_address="127.0.0.1";
    unsigned short port=3333;
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),port);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc,ep.protocol());
        sock.connect(ep);
        std::string buf="helloworld";
        int send_length= sock.send(asio::buffer(buf.c_str(),buf.length()));
        if (send_length<=0)
        {
            std::cout<<"send error"<<std::endl;
        }
    }catch (system::system_error &e)
    {
        std::cout<<e.what()<<std::endl;
        return -1;
    }
    return 0;
}


int seng_data_by_write()
{
    std::string raw_ip_address="127.0.0.1";
    unsigned short port=3333;
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),port);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc,ep.protocol());
        sock.connect(ep);
        std::string buf="helloworld";
        int send_length= asio::write(sock,asio::buffer(buf.c_str(),buf.length()));
        if(send_length<=0)
        {
            return 0;
        }

    }catch (system::system_error &e)
    {
        std::cout<<e.what()<<std::endl;
        return -1;
    }
    return 0;
}

//同步读操作
std::string read_from_socket(asio::ip::tcp::socket&sock)
{
    const unsigned char MESSAGE_SIZE=7;
    char buf[MESSAGE_SIZE];
    //表示我们读了几个字节
    std::size_t total_bytes_read=0;
    while (total_bytes_read!=MESSAGE_SIZE)
    {
        total_bytes_read +=sock.read_some(asio::buffer(buf+total_bytes_read,MESSAGE_SIZE-total_bytes_read));
    }
    return std::string(buf,total_bytes_read);
}
//socket使用同步读操作
int read_data_by_read_some()
{
    std::string raw_ip_address="127.0.0.1";
    unsigned short port=3333;
    try
    {
        asio::ip::tcp::ep(asio::ip::address::from_string(raw_ip_address),port);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc,ep.protocol());
        sock.connect(ep);
        read_from_socket(sock);
    }catch (system::system_error&e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

int read_data_by_recive()
{
    std::string raw_ip_address="127.0.0.1";
    unsigned short port=3333;
    try
    {
        asio::ip::tcp::ep(asio::ip::address::from_string(raw_ip_address),port);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc,ep.protocol());
        sock.connect(ep);
        const unsigned char MESSAGE_SIZE=7;
        char buf_recive[MESSAGE_SIZE];
        int receive_length=sock.receive(asio::buffer(buf_recive,MESSAGE_SIZE));
        if (receive_length<=0)
        {
            std::cout<<"receive error"<<std::endl;
        }
    }catch (system::system_error&e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

int read_data_by_read()
{
    std::string raw_ip_address="127.0.0.1";
    unsigned short port=3333;
    try
    {
        asio::ip::tcp::ep(asio::ip::address::from_string(raw_ip_address),port);
        asio::io_context ioc;
        asio::ip::tcp::socket sock(ioc,ep.protocol());
        sock.connect(ep);
        const unsigned char MESSAGE_SIZE=7;
        char buf_recive[MESSAGE_SIZE];
        int receive_length=asio::read(sock,asio::buffer(buf_recive,MESSAGE_SIZE));
        if (receive_length<=0)
        {
            std::cout<<"receive error"<<std::endl;
        }
    }catch (system::system_error&e)
    {
        std::cout<<e.what()<<std::endl;
    }
}