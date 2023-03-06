#ifndef USER_H
#define USER_H
#include <string>
#include <jsoncpp/json/json.h>

class User
{
public:
    User(Json::Value &userinfojson);
    ~User();

    Json::Value getUserInfo();
    // 获取用户昵称
    std::string getNickName();

private:
    std::string m_userid;
    std::string m_avatar;
    std::string m_nickname;
    std::string m_account;
    std::string m_password;
};
#endif