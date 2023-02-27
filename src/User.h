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

private:
    std::string m_userid;
    std::string m_avator;
    std::string m_nickname;
    std::string m_account;
    std::string m_password;
};
#endif