#include "StatusRecord.h"
#include "MongoDataBase.h"

StatusRecord &StatusRecord::GetInstance()
{
    static StatusRecord statusrecord;
    return statusrecord;
}

Json::Value StatusRecord::SelectStatusRecordInfo(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectStatusRecord(queryjson);
}
StatusRecord::StatusRecord()
{
}
StatusRecord::~StatusRecord()
{
}