#include "Discuss.h"
#include "MongoDataBase.h"

Disscuss &Disscuss::GetInstance()
{
    static Disscuss disscuss;
    return disscuss;
}
Json::Value Disscuss::getAllDiscuss()
{
    return MoDB::GetInstance().getAllDiscuss();
}

Json::Value Disscuss::InsertDiscuss(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertDiscuss(insertjson);
}

Json::Value Disscuss::SelectDiscuss(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscuss(queryjson);
}

Json::Value Disscuss::SelectDiscussContent(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscussContent(queryjson);
}

bool Disscuss::UpdateDiscussComments(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateDiscussComments(updatejson);
}

Disscuss::Disscuss()
{
}
Disscuss::~Disscuss()
{
}