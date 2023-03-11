#ifndef USERSET_H
#define USERSET_H
#include <unordered_map>
#include <jsoncpp/json/json.h>

class User;
class UserSet
{
public:
    // 局部静态特性的方式实现单实例
    static UserSet &GetInstance();
    // 注册用户
    Json::Value RegisterUser(Json::Value &registerjson);
    // 登录用户
    Json::Value LoginUser(Json::Value &loginjson);

private:
    UserSet();
    ~UserSet();

private:
};
#endif