#include "UserSet.h"
#include "DataBase.h"
#include "User.h"

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

UserSet::UserSet()
{
}
UserSet::~UserSet()
{
}