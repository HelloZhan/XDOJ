#ifndef CONTROL_H
#define CONTROL_H
#include <jsoncpp/json/json.h>
#include <string>
#include "Judger.h"
class Control
{
public:
    // 返回具体题目
    std::string GetProblemDescription(std::string problemid);

    // 返回题库
    Json::Value GetProblemSet(std::string querytype, std::string page, std::string pagesize, std::string matchstring);

    // 返回判题信息
    Json::Value GetJudgeCode(Json::Value judgejson);

    // 返回状态记录的信息
    Json::Value GetStatusRecordInfo(Json::Value &queryjson);
    Control();

    ~Control();

private:
    Judger judger;
    /* data */
};

#endif