// #include"db.h"
// #include<muduo/base/Logging.h>
// // 数据库配置信息
// static string server = "127.0.0.1";
// static string user = "root";
// static string password = "123456";
// static string dbname = "chat";
// // 初始化数据库连接
// MySQL::MySQL()
// {
//     _conn = mysql_init(nullptr);
// }
// // 释放数据库连接资源
// MySQL::~MySQL()
// {
//     if (_conn != nullptr)
//         mysql_close(_conn);
// }
// // 连接数据库
// bool MySQL::connect()
// {
//     MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
//        password.c_str(), dbname.c_str(), 3306, nullptr, 0);
//     if (p != nullptr)
//     {
//         //c和c++代码默认的编码字符是ASCII,如果不设置从mysql上拉下来的中文显示？？
//         mysql_query(_conn, "set names gbk");
//         LOG_INFO << "Connected mysql success";
//     }
//     else
//     {
//         LOG_ERROR << "Connect failed";
//     }
//     return p;
// }
// // 更新操作
// bool MySQL::update(string sql)
// {
//     if (mysql_query(_conn, sql.c_str()))
//     {
//         LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
//             << sql << "更新失败!";
//         return false;
//     }
//     return true;
// }
// // 查询操作
// MYSQL_RES* MySQL::query(string sql)
// {
//     if (mysql_query(_conn, sql.c_str()))
//     {
//         LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
//             << sql << "查询失败!";
//         return nullptr;
//     }
//     return mysql_use_result(_conn);
// }
// //获取连接
// MYSQL* MySQL::getConnection()
// {
//     return _conn;
// }
#include"db.h"
#include<muduo/base/Logging.h>
#include<mysql/mysql.h>
#include<string>

// 数据库配置信息
static std::string server = "192.168.244.128";
static std::string user = "root";
static std::string password = "123456";
static std::string dbname = "chat";

// 初始化数据库连接
MySQL::MySQL()
{
    _conn = mysql_init(nullptr);
    if (_conn == nullptr)
    {
        LOG_ERROR << "mysql_init failed";
    }
}

// 释放数据库连接资源
MySQL::~MySQL()
{
    if (_conn != nullptr)
        mysql_close(_conn);
}

// 连接数据库
bool MySQL::connect()
{
    if (_conn == nullptr)
    {
        LOG_ERROR << "mysql_init not initialized";
        return false;
    }

    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
       password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        // 设置编码为utf8mb4，支持中文和emoji
        mysql_query(_conn, "set names utf8mb4");
        LOG_INFO << "Connected mysql success";
    }
    else
    {
        LOG_ERROR << "Connect failed: " << mysql_error(_conn);
    }
    return p != nullptr;
}

// 更新操作
bool MySQL::update(std::string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_ERROR << __FILE__ << ":" << __LINE__ << ":"
            << sql << "更新失败! 错误信息: " << mysql_error(_conn);
        return false;
    }
    return true;
}

// 查询操作
MYSQL_RES* MySQL::query(std::string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_ERROR << __FILE__ << ":" << __LINE__ << ":"
            << sql << "查询失败! 错误信息: " << mysql_error(_conn);
        return nullptr;
    }
    return mysql_use_result(_conn);
}

// 获取连接
MYSQL* MySQL::getConnection()
{
    return _conn;
}
