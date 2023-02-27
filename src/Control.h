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
    Json::Value GetProblemSet();

    // 返回判题信息
    Json::Value GetJudgeCode(Json::Value judgejson);

    Control();

    ~Control();

private:
    Judger judger;
    /* data */
};

#endif