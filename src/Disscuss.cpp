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

bool Disscuss::InsertDiscuss(Json::Value &insertjson)
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

Disscuss::Disscuss()
{
}
Disscuss::~Disscuss()
{
}