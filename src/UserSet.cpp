#include "UserSet.h"
#include "MongoDataBase.h"
#include <iostream>
using namespace std;

UserSet &UserSet::GetInstance()
{
    static UserSet userset;
    return userset;
}
Json::Value UserSet::RegisterUser(Json::Value &registerjson)
{
    return MoDB::GetInstance().RegisterUser(registerjson);
}

Json::Value UserSet::LoginUser(Json::Value &loginjson)
{
    return MoDB::GetInstance().LoginUser(loginjson);
}

bool UserSet::UpdateUserProblemInfo(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateUserProblemInfo(updatejson);
}

UserSet::UserSet()
{
}
UserSet::~UserSet()
{
}