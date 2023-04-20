#include "UserList.h"
#include "MongoDataBase.h"
#include <iostream>
using namespace std;

UserList *UserList::GetInstance()
{
    static UserList userlist;
    return &userlist;
}
Json::Value UserList::RegisterUser(Json::Value &registerjson)
{
    Json::Value json = MoDB::GetInstance()->RegisterUser(registerjson);
    // 将其权限加入用户权限表中
    if (json["Reuslt"].asString() == "Success")
    {
        int64_t id = json["_id"].asInt64();
        UserAuthorityMap[id] = 3;
    }
    return json;
}

Json::Value UserList::LoginUser(Json::Value &loginjson)
{
    return MoDB::GetInstance()->LoginUser(loginjson);
}

bool UserList::UpdateUserProblemInfo(Json::Value &updatejson)
{
    return MoDB::GetInstance()->UpdateUserProblemInfo(updatejson);
}

Json::Value UserList::SelectUserRank(Json::Value &queryjson)
{
    return MoDB::GetInstance()->SelectUserRank(queryjson);
}

Json::Value UserList::SelectUserInfo(Json::Value &queryjson)
{
    return MoDB::GetInstance()->SelectUserInfo(queryjson);
}

Json::Value UserList::UpdateUserInfo(Json::Value &updatejson)
{
    return MoDB::GetInstance()->UpdateUserInfo(updatejson);
}

Json::Value UserList::SelectUserUpdateInfo(Json::Value &queryjson)
{
    return MoDB::GetInstance()->SelectUserUpdateInfo(queryjson);
}

Json::Value UserList::SelectUserSetInfo(Json::Value &queryjson)
{
    return MoDB::GetInstance()->SelectUserSetInfo(queryjson);
}

Json::Value UserList::DeleteUser(Json::Value &deletejson)
{
    Json::Value json = MoDB::GetInstance()->DeleteUser(deletejson);
    if (json["Result"].asString() == "Success")
    {
        int64_t id = stoll(deletejson["UserId"].asString());
        UserAuthorityMap.erase(id);
    }
    return json;
}

bool UserList::InitUserAuthority()
{
    Json::Value json = MoDB::GetInstance()->SelectUserAuthority();
    if (json["Result"] == "Fail")
        return false;

    for (auto info : json["ArrayInfo"])
    {
        UserAuthorityMap[info["_id"].asInt64()] = info["Authority"].asInt();
    }

    return true;
}

int UserList::GetUserAuthority(Json::Value &json)
{
    /*
        用户权限
        1：游客
        3：普通用户
        5：管理员
    */
    // 如果未发现ID
    if (json["VerifyId"].isNull())
        return 1;
    try
    {
        int64_t id = stoll(json["VerifyId"].asString());
        // 如果未查询到该用户ID或者用户ID为0是游客
        if (UserAuthorityMap[id] == 0)
            return 1;
        return UserAuthorityMap[id];
    }
    catch (const std::exception &e)
    {
        return 1;
    }
}
// 是否是普通用户
bool UserList::IsOrdinaryUser(Json::Value &json)
{
    if (GetUserAuthority(json) >= 3)
        return true;
    else
        return false;
}

// 是否是作者本人
bool UserList::IsAuthor(Json::Value &json)
{
    if (GetUserAuthority(json) < 3)
        return false;
    try
    {
        int64_t verifyid = stoll(json["VerifyId"].asString());
        int64_t userid = stoll(json["UserId"].asString());

        if (verifyid == userid)
            return true;
        else
            return false;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

// 是否是管理员
bool UserList::IsAdministrator(Json::Value &json)
{
    if (GetUserAuthority(json) >= 5)
        return true;
    else
        return false;
}

UserList::UserList()
{
}
UserList::~UserList()
{
}