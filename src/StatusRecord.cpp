#include "StatusRecord.h"
#include "MongoDataBase.h"
#include <string>

using namespace std;

StatusRecord &StatusRecord::GetInstance()
{
    static StatusRecord statusrecord;
    return statusrecord;
}

Json::Value StatusRecord::SelectStatusRecordInfo(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectStatusRecord(queryjson);
}

string StatusRecord::InsertStatusRecord(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertStatusRecord(insertjson);
}

Json::Value StatusRecord::UpdateStatusRecord(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateStatusRecord(updatejson);
}

Json::Value StatusRecord::SelectOneStatusRecord(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectOneStatusRecord(queryjson);
}

StatusRecord::StatusRecord()
{
}

StatusRecord::~StatusRecord()
{
}