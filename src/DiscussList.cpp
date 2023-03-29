#include "DiscussList.h"
#include "MongoDataBase.h"

DiscussList &DiscussList::GetInstance()
{
    static DiscussList disscusslist;
    return disscusslist;
}

Json::Value DiscussList::InsertDiscuss(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertDiscuss(insertjson);
}

Json::Value DiscussList::SelectDiscussList(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscussList(queryjson);
}

Json::Value DiscussList::SelectDiscussListByAdmin(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscussListByAdmin(queryjson);
}

Json::Value DiscussList::SelectDiscussByEdit(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscussByEdit(queryjson);
}

Json::Value DiscussList::SelectDiscuss(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscuss(queryjson);
}

bool DiscussList::UpdateDiscussComments(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateDiscussComments(updatejson);
}

Json::Value DiscussList::UpdateDiscuss(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateDiscuss(updatejson);
}

Json::Value DiscussList::DeleteDiscuss(Json::Value &deletejson)
{
    return MoDB::GetInstance().DeleteDiscuss(deletejson);
}
DiscussList::DiscussList()
{
}
DiscussList::~DiscussList()
{
}