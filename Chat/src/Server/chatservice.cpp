#include"chatservice.hpp"
#include"public.hpp"
#include<muduo/base/Logging.h>
using namespace muduo;
using namespace std;
//获取单例对象的接口函数
ChatService* ChatService::instance()
{
    static ChatService instance;
    return &instance;
}

//注册消息以及对应的Handler回调操作
ChatService::ChatService()
{
    msgHandlerMap_.insert({LOGIN_MSG,std::bind(&ChatService::login,this,_1,_2,_3)});
    msgHandlerMap_.insert({REG_MSG,std::bind(&ChatService::reg,this,_1,_2,_3)});
}

MsgHandler ChatService::getHandler(int msgid)
{
    //记录错误日志，msgid没有对应事件处理
    auto it=msgHandlerMap_.find(msgid);
    if(it==msgHandlerMap_.end())
    {
        //LOG_ERROR<<"msgid:"<<msgid<<"can't find Handler";
        //string errstr="msgid"+msgid+"can't find Handler";
        //返回体格默认的处理器。空操作
        return [=](const TcpConnectionPtr&conn,json&js,Timestamp)
        {
            LOG_ERROR<<"msgid:"<<msgid<<"can't find Handler";
        };
    }
    else
    {
       return msgHandlerMap_[msgid];
    }

}


//处理登录业务    ORM(对象框架映射) 业务操作层的都是对象  DAO(数据层)
 void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time)
 {
     int id=js["id"].get<int>();
     string pwd=js["password"];

     User user=usermodel_.query(id);
     if (user.getId()==id&&user.getPassword()==pwd)
     {
         if (user.getState()=="online")
         {
             //用户已经登录，不允许重复登录
             json response;
             response["msgid"]=LOGIN_MSG_ACK;
             response["errno"]=2;
             response["errmsg"]="账号已经登录，重新登录其他账号";
             conn->send(response.dump());
         }
         else
         {
             //登录成功，更新用户状态信息 state offline->online
             user.setState("online");
             usermodel_.updateState(user);
             json response;
             response["msgid"]=LOGIN_MSG_ACK;
             response["errno"]=0;
             response["id"]=user.getId();
             response["name"]=user.getName();
             conn->send(response.dump());
         }

     }
     else
     {
         //该用户不存在 用户存在但是密码错误登录失败
         json response;
         response["msgid"]=LOGIN_MSG_ACK;
         response["errno"]=1;
         response["errmsg"]="用户名或者密码错误";
         conn->send(response.dump());
     }
 }


//处理注册业务  name passward
void ChatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPassword(pwd);
    bool state=usermodel_.insert(user);
    if(state)
    {
        //表示注册成功
        json response;
        response["msgid"]=REG_MSG_ACK;
        response["errno"]=0;
        response["id"]=user.getId();
        conn->send(response.dump());
    }
    else
    {
        //注册失败
        json response;
        response["msgid"]=REG_MSG_ACK;
        response["errno"]=1;
        conn->send(response.dump());
    }
}

