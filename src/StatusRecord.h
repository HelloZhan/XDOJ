#ifndef STATUSRECORD_H
#define STATUSRECORD_H
#include <jsoncpp/json/json.h>
#include <string>
// 状态记录类，保存代码判定结果的类
class StatusRecord
{
public:
    // 局部静态特性的方式实现单实例
    static StatusRecord &GetInstance();

    // 分页查询测评记录
    Json::Value SelectStatusRecordInfo(Json::Value &queryjson);

    // 插入查询记录
    std::string InsertStatusRecord(Json::Value &insertjson);

    // 更新测评信息返回测评结果
    Json::Value UpdateStatusRecord(Json::Value &updatejson);

    // 查询一条详细测评记录
    Json::Value SelectOneStatusRecord(Json::Value &queryjson);

private:
    StatusRecord();
    ~StatusRecord();
};

#endif