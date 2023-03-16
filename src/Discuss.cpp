#include "Discuss.h"
#include "MongoDataBase.h"

Discuss &Discuss::GetInstance()
{
    static Discuss disscuss;
    return disscuss;
}
Json::Value Discuss::getAllDiscuss()
{
    return MoDB::GetInstance().getAllDiscuss();
}

Json::Value Discuss::InsertDiscuss(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertDiscuss(insertjson);
}

Json::Value Discuss::SelectDiscuss(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscuss(queryjson);
}

Json::Value Discuss::SelectDiscussContent(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscussContent(queryjson);
}

bool Discuss::UpdateDiscussComments(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateDiscussComments(updatejson);
}

Json::Value Discuss::UpdateDiscuss(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateDiscuss(updatejson);
}

Discuss::Discuss()
{
}
Discuss::~Discuss()
{
}