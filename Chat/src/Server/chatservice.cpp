#include"chatservice.hpp"
#include"public.hpp"
#include<muduo/base/Logging.h>
using namespace muduo;

ChatService* ChatService::instance()
{
    //获取单例对象的接口函数
    static ChatService service;
    return &service;
}

//注册方法以对应的Handler回调操作
ChatService::ChatService()
{
    msgHandlerMap_.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    msgHandlerMap_.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});
}

MsgHandler ChatService::getHandler(int msgid)
{
    //记录错误日志，msgid没有对应事件处理
    auto it = msgHandlerMap_.find(msgid);
    if (it == msgHandlerMap_.end())
    {
        //LOG_ERROR<<"msgid:"<<msgid<<"can't find Handler";
        //string errstr="msgid"+msgid+"can't find Handler";
        //返回一个默认的处理器。空操作
        return [=](const TcpConnectionPtr& conn, json& js, Timestamp)
        {
            LOG_ERROR << "msgid:" << msgid << "can't find Handler";
        };
    }
    else
    {
        return msgHandlerMap_[msgid];
    }
}

//处理登录业务
void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    LOG_INFO << "login";
}

//处理注册业务
void ChatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    LOG_INFO << "reg";
}