#include "Session.h"
#include<iostream>
#include<exception>
using namespace std;
void Session::Start()
{
    memset(_data,0,max_length);
    _socket.async_read_some(boost::asio::buffer(_data,max_length),
        std::bind(&Session::handle_read,this,std::placeholders::_1,std::placeholders::_2,shared_from_this()));
}
void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred,shared_ptr<Session>self_shared)
{
    if (!error)
    {
        cout<<"server receive data is "<<_data<<endl;
        boost::asio::async_write(_socket,boost::asio::buffer(_data,bytes_transferred),
            std::bind(&Session::handle_write,this,placeholders::_1,self_shared));
    }
    else
    {
        cout<<"read error"<<endl;
        _server->ClearSession(_uuid);
    }
}

std::string& Session::GetUuid()
{
    return _uuid;
}

void Session::handle_write(const boost::system::error_code& error,shared_ptr<Session>self_shared)
{
    if (!error)
    {
        memset(_data,0,max_length);
        _socket.async_read_some(boost::asio::buffer(_data,max_length),
            std::bind(&Session::handle_read,this,std::placeholders::_1,std::placeholders::_2,self_shared));
    }
    else
    {
        cout<<"handle_write"<<error.value()<<endl;
        _server->ClearSession(_uuid);
    }
}

Server::Server(boost::asio::io_context&ioc,short port):_ioc(ioc),_acceptor(ioc,tcp::endpoint(tcp::v4(),port))
{
    cout<<"server start"<<endl;
    start_accept();
}
void Server::start_accept()
{
    shared_ptr<Session>new_session=make_shared<Session>(_ioc,this);
    _acceptor.async_accept(new_session->Socket(),std::bind(&Server::handle_accept,this,new_session,
        placeholders::_1));
}

void Server::handle_accept(shared_ptr<Session>new_session,const boost::system::error_code& error)
{
    if (!error)
    {
        new_session->Start();
        _sessions.insert(make_pair(new_session->GetUuid(),new_session));
    }
    else
    {

    }
    start_accept();
}

void Server::ClearSession(std::string uuid)
{
    _sessions.erase(uuid);
}


int main()
{
    try
    {
        boost::asio::io_context  ioc;
        using namespace std;
        Server s(ioc,10086);
        ioc.run();
    }catch(std::exception&e)
    {
        std::cerr<<"Exception"<<e.what()<<endl;
    }
    return 0;
}