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
Disscuss::Disscuss()
{
}
Disscuss::~Disscuss()
{
}