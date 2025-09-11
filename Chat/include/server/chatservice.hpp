#ifndef CHATSERVICE_H
#define CHATSERVICE_H
#include"chatserver.hpp"
#include<unordered_map>
#include<muduo/net/TcpConnection.h>
#include<functional>
#include"json.hpp"
#include"usermodel.hpp"
#include<mutex>
#include"offlinemessagemodel.hpp"
#include"friendmodel.hpp"
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json=nlohmann::json;

//表示处理消息的事件回调方法类型
using MsgHandler=std::function<void(const TcpConnectionPtr&conn,json&js,Timestamp)>;

//聊天服务器业务类
class ChatService
{
public:
    //获取单例对象的接口函数
    static ChatService* instance();
    //处理登录业务
    void login(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //一对一来那天业务
    void oneChat(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //添加好友业务
    void addFriend(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //服务器异常后业务重置方法
    void reset();
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);

private:
    ChatService();
    //村塾消息id和其对应业务处理方法
    unordered_map<int,MsgHandler>msgHandlerMap_;

    //存储在线用户的通信连接(设计用户上下线要保证线程安全)
    unordered_map<int,TcpConnectionPtr>userConnMap_;

    //定义互斥锁保证userConnMap_的线程安全
    mutex connMutex_;

    //数据操作类对象
    UserModel userModel_;
    OfflineMsgModel offlineMsgModel_;
    FriendModel friendModel_;
};

#endif