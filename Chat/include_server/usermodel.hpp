#ifndef USER_MODEL_H
#define USER_MODEL_H
#include "user.hpp"

//user表的操作类
class usermodel
{
public:
    //user表的增加方方法
    bool insert(User &user);

    //根据用户号码查询用户信息
    User query(int id);

    //更新用户状态信息
    bool updateState(User user);
};
#endif