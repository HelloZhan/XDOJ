#ifndef CONTROL_H
#define CONTROL_H
#include <jsoncpp/json/json.h>
#include <string>
#include "Judger.h"
class Control
{
public:
    // ----------------用户表 User-----------------
    // 注册用户
    Json::Value RegisterUser(Json::Value &registerjson);

    // 登录用户
    Json::Value LoginUser(Json::Value &loginjson);

    // 获取用户Rank排名
    Json::Value SelectUserRank(Json::Value &queryjson);

    // ---------------题目 Problem -------------------
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

    // 分页查询讨论
    Json::Value SelectDiscuss(Json::Value &queryjson);

    // 获取讨论的内容
    Json::Value SelectDiscussContent(Json::Value &queryjson);

    // 获取评论 根据Id
    Json::Value GetComment(Json::Value &queryjson);

    // 插入评论
    Json::Value InsertComment(Json::Value &insertjson);

    Control();

    ~Control();

private:
    Judger judger;
    /* data */
};

#endif