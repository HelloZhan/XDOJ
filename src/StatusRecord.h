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

    Json::Value getStatusRecordInfo(Json::Value queryjson);

private:
    StatusRecord();
    ~StatusRecord();
};

#endif