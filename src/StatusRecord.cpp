#include "StatusRecord.h"
#include "MySQLDataBase.h"

StatusRecord &StatusRecord::GetInstance()
{
    static StatusRecord statusrecord;
    return statusrecord;
}

Json::Value StatusRecord::SelectStatusRecordInfo(Json::Value &queryjson)
{
    return MyDB::GetInstance().SelectStatusRecordInfo(queryjson);
}
StatusRecord::StatusRecord()
{
}
StatusRecord::~StatusRecord()
{
}