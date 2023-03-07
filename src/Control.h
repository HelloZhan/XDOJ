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
    Json::Value SelectProblemSetInfo(Json::Value &queryjson);

    // 返回判题信息
    Json::Value GetJudgeCode(Json::Value judgejson);

    // 返回状态记录的信息
    Json::Value SelectStatusRecordInfo(Json::Value &queryjson);

    // 讨论

    // 获取所有讨论
    Json::Value GetAllDiscuss();

    // 获取评论 根据Id
    Json::Value GetAllCommentById(std::string &id);

    Control();

    ~Control();

private:
    Judger judger;
    /* data */
};

#endif