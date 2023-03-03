#include "UserSet.h"
#include "MySQLDataBase.h"
#include "User.h"
#include <iostream>
using namespace std;

UserSet &UserSet::GetInstance()
{
    static UserSet userset;
    return userset;
}

bool UserSet::LoadDataBaseInfo()
{
    Json::Value UserInfo = MyDB::GetInstance().getAllUserInfo();
    int UserNum = (int)UserInfo["Array"].size();
    for (int i = 0; i < UserNum; i++)
    {
        User *tmp = new User(UserInfo["Array"][i]);
        heap[UserInfo["Array"][i]["UserId"].asString()] = tmp;
    }
    return true;
}

Json::Value UserSet::getUserInfoById(std::string userid)
{
    return heap[userid]->getUserInfo();
}

string UserSet::getUserNickNameById(string userid)
{
    return heap[userid]->getNickName();
}

UserSet::UserSet()
{
}
UserSet::~UserSet()
{
}