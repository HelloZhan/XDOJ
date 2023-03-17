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

    // 获取用户大部分信息
    Json::Value SelectUserInfo(Json::Value &queryjson);

    // 更新用户信息
    Json::Value UpdateUserInfo(Json::Value &updatejson);
    // ---------------题目 Problem -------------------
    // 返回具体题目
    std::string GetProblemDescription(std::string problemid);

    // 返回题库
    Json::Value SelectProblemSetInfo(Json::Value &queryjson);

    // 返回判题信息
    Json::Value GetJudgeCode(Json::Value judgejson);

    // 返回状态记录的信息
    Json::Value SelectStatusRecordInfo(Json::Value &queryjson);

    // ---------------评论Comment-------------------
    // 获取评论 根据Id
    Json::Value GetComment(Json::Value &queryjson);

    // 插入评论
    Json::Value InsertComment(Json::Value &insertjson);

    // 删除评论
    Json::Value DeleteComment(Json::Value &deletejson);

    // -------文章 Article （包含Solution，Discuss，Announcement）---------

    // 分页查询讨论
    Json::Value SelectArticle(Json::Value &queryjson);

    // 获取讨论的内容
    Json::Value SelectArticleContent(Json::Value &queryjson);

    // 插入文章（讨论，题解）
    Json::Value InsertArticle(Json::Value &insertjson);

    // 更新文章
    Json::Value UpdateArticle(Json::Value &updatejson);

    // 删除文章
    Json::Value DeleteArticle(Json::Value &deletejson);
    Control();

    ~Control();

private:
    Judger judger;
    /* data */
};

#endif