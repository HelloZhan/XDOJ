#include "StatusRecord.h"
#include "DataBase.h"

StatusRecord &StatusRecord::GetInstance()
{
    static StatusRecord statusrecord;
    return statusrecord;
}

Json::Value StatusRecord::getStatusRecordInfo(Json::Value queryjson)
{
    return MyDB::GetInstance().getStatusRecordInfo(queryjson);
}
StatusRecord::StatusRecord()
{
}
StatusRecord::~StatusRecord()
{
}