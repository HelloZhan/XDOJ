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

    // 获取用户信息，以供修改
    Json::Value SelectUserUpdateInfo(Json::Value &queryjson);

    // 分页获取用户信息
    Json::Value SelectUserSetInfo(Json::Value &queryjson);

    // 删除用户
    Json::Value DeleteUser(Json::Value &deletejson);
    // ---------------题目 Problem -------------------

    // 管理员查看题目数据
    Json::Value SelectProblemInfoByAdmin(Json::Value &queryjson);

    // 编辑题目
    Json::Value EditProblem(Json::Value &insertjson);

    // 删除题目
    Json::Value DeleteProblem(Json::Value &deletejson);

    // 返回具体题目
    std::string GetProblemDescription(std::string problemid);

    // 返回题库
    Json::Value SelectProblemSetInfo(Json::Value &queryjson);

    // 返回判题信息
    Json::Value GetJudgeCode(Json::Value judgejson);

    // ---------------测评表-------------------------
    // 返回状态记录的信息
    Json::Value SelectStatusRecordInfo(Json::Value &queryjson);

    // 查询一条详细测评记录
    Json::Value SelectOneStatusRecord(Json::Value &queryjson);

    // ---------------评论Comment-------------------
    // 获取评论 根据Id
    Json::Value GetComment(Json::Value &queryjson);

    // 插入评论
    Json::Value InsertComment(Json::Value &insertjson);

    // 删除评论
    Json::Value DeleteComment(Json::Value &deletejson);

    // ----------------------公告---------------------------
    // 查询公告列表
    Json::Value SelectAnnouncement(Json::Value &queryjson);

    // 查询公告
    Json::Value SelectAnnouncementContent(Json::Value &queryjson);

    // 查询公告 进行编辑
    Json::Value SelectAnnouncementByEdit(Json::Value &queryjson);

    // 插入公告
    Json::Value InsertAnnouncement(Json::Value &insertjson);

    // 更新公告
    Json::Value UpdateAnnouncement(Json::Value &updatejson);

    // 删除公告
    Json::Value DeleteAnnouncement(Json::Value &deletejson);

    // ----------------------题解-----------------------
    // 查询题解列表
    Json::Value SelectSolution(Json::Value &queryjson);

    // 管理员查询
    Json::Value SelectSolutionByAdmin(Json::Value &queryjson);

    // 查询题解
    Json::Value SelectSolutionContent(Json::Value &queryjson);

    // 查询题解进行编辑
    Json::Value SelectSolutionByEdit(Json::Value &queryjson);

    // 插入题解
    Json::Value InsertSolution(Json::Value &insertjson);

    // 更新题解
    Json::Value UpdateSolution(Json::Value &updatejson);

    // 删除题解
    Json::Value DeleteSolution(Json::Value &deletejson);

    // ----------------------讨论-----------------------
    // 查询题解列表
    Json::Value SelectDiscuss(Json::Value &queryjson);

    // 管理员查询
    Json::Value SelectDiscussByAdmin(Json::Value &queryjson);

    // 查询题解
    Json::Value SelectDiscussContent(Json::Value &queryjson);

    // 查询题解进行编辑
    Json::Value SelectDiscussByEdit(Json::Value &queryjson);

    // 插入题解
    Json::Value InsertDiscuss(Json::Value &insertjson);

    // 更新题解
    Json::Value UpdateDiscuss(Json::Value &updatejson);

    // 删除题解
    Json::Value DeleteDiscuss(Json::Value &deletejson);

    Json::Value GetTags(Json::Value &queryjson);
    Control();

    ~Control();

private:
    Judger judger;
    /* data */
};

#endif