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

    // 加载数据库信息
    bool LoadDataBaseInfo();

    // 获取某用户信息
    Json::Value getUserInfoById(std::string userid);

    // 获取某用户昵称
    std::string getUserNickNameById(std::string userid);

private:
    UserSet();
    ~UserSet();

private:
    std::unordered_map<std::string, User *> heap;
};
#endif