#include<memory>
#include<boost/asio.hpp>
#include<iostream>
#include <queue>
using namespace boost;
using namespace std;
const int RECVSIZE=1024;
class MsgNode
{
public:
    MsgNode(const char* msg,int total_len):_total_len(total_len),_cur_len(0)
    {
        _msg=new char[_total_len];
        memcpy(_msg,msg,_total_len);
    }
    MsgNode(int total_len):_total_len(total_len),_cur_len(0)
    {
        _msg=new char[_total_len];
    }
    ~MsgNode()
    {
        delete[]_msg;
    }
private:
    int _total_len;
    int _cur_len;
    char* _msg;
};
class Session
{
public:
    Session(std::shared_ptr<asio::ip::tcp::socket>socket);
    void Connect(const asio::ip::tcp::endpoint&ep);
    void WriteCallBack(const boost::system::error_code&ec,std::size_t bytes_transferred);
    void WriteToSocket(const std::string&buf);

    void WriteAllToSocket(const std::string&buf);
    void WriteAllCallBack(const boost::system::error_code&ec,std::size_t byted_transferred);
private:
    std::shared_ptr<asio::ip::tcp::socket>_socket;
    std::queue<std::shared_ptr<MsgNode>> _send_queue;
    bool _send_pending;
};
Session::Session(std::shared_ptr<asio::ip::tcp::socket> socket):_socket(socket),_send_pending(false){}
void Session::Connect(const asio::ip::tcp::endpoint& ep)
{
    _socket->connect(ep);
}
void Session::WriteCallBack(const boost::system::error_code&ec,std::size_t bytes_transferred)
{
    if (ec.value()!=0)
    {
        std::cout<<ec.value()<<std::endl;
    }
    auto&send_data=_send_queue.front();
    send_data->_cur_len+=bytes_transferred;
    if (send_data->_cur_len<send_data>_total_len)
    {
        this->_socket->async_write_some(asio::buffer(send_data->_msg+send_data->_cur_len,
            send_data->_total_len-send_data->_cur_len),
            std::bind(&Session::WriteCallBack,this,std::placeholders::_1,std::placeholders::_2));
            return;
    }
    _send_queue.pop();
    if (_send_queue.empty())
    {
        _send_pending=false;
    }
    if (!_send_queue.empty())
    {
        auto&send_data=_send_queue.front();
        this->_socket->async_write_some(asio::buffer(send_data->_msg+_send_node->_cur_len,
            send_data->_total_len-_send_node->_cur_len),
            std::bind(&Session::WriteCallback,this,std::placeholders::_1,std::placeholders::_2));
    }
}
void Session::WriteToSocket(const std::string&buf)
{
    _send_queue.emplace(new MsgNode(buf.c_str(),buf.length()));
    if (_send_pending)
    {
        return;
    }
    this->_socket->async_write_some(asio::buffer(buf),
        std::bind(&Session::WriteToSocket,this,std::placeholders::_1,std::placeholders::_2));
    _send_pending=true;
}

void Session::WriteAllToSocket(const std::string&buf)
{
    _send_queue.emplace(new MsgNode(buf.c_str(),buf.length()));
    if (_send_pending)
    {
        return;
    }
    this->_socket->async_send(asio::buffer(buf),std::bind(&Session::WriteAllCallBack,this,
        std::placeholders::_1,std::placeholders::_2));
    _send_pending=true;
}

void Session::WriteAllCallBack(const boost::system::error_code&ec,std::size_t bytes_transferred)
{
    if (ec.value()!=0)
    {
        std::cout<<"Error occured! Error code="<<ec.value()<<ec.message();
        return;
    }
    _send_queue.pop();
    if (_send_queue.empty())
    {
        _send_pending=false;
    }
    if (!_send_queue.empty())
    {
        auto&send_data=_send_queue.front();
        this->_socket->async_send(asio::buffer(buf),std::bind(&Session::WriteAllCallBack,this,
        std::placeholders::_1,std::placeholders::_2));
        
    }
}