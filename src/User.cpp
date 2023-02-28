#include "User.h"

using namespace std;
User::User(Json::Value &userinfojson)
{
    m_userid = userinfojson["UserId"].asString();
    m_avator = userinfojson["Avator"].asString();
    m_nickname = userinfojson["NickName"].asString();
    m_account = userinfojson["Account"].asString();
    m_password = userinfojson["Password"].asString();
}
User::~User()
{
}

Json::Value User::getUserInfo()
{
    Json::Value resjson;
    resjson["UserId"] = m_userid;
    resjson["Avator"] = m_avator;
    resjson["NickName"] = m_nickname;

    return resjson;
}

string User::getNickName()
{
    return m_nickname;
}