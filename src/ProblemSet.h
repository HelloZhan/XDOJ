#ifndef PROBLEMSET_H
#define PROBLEMSET_H

#include <map>
#include <string>
#include <jsoncpp/json/json.h>
class Problem;

#define PJ "Pending & Judging"
#define CE "Compile Error"
#define AC "Accepted"
#define WA "Wrong Answer"
#define RE "Runtime Error"
#define TLE "Time Limit Exceeded"
#define MLE "Memory Limit Exceeded"
#define SE "System Error"
class ProblemSet
{
public:
    // 局部静态特性的方式实现单实例
    static ProblemSet &GetInstance();

    void Init();

    // 管理员查询题目详细信息
    Json::Value SelectProblemInfoByAdmin(Json::Value &queryjson);

    // 插入题目
    Json::Value InsertProblem(Json::Value &insertjson);

    // 获取题目描述
    std::string getProblemDescription(std::string id);

    // 通过普通查询获取题库数据
    Json::Value SelectProblemSetInfo(Json::Value &queryjson);

    // 更新题目状态数量
    bool UpdateProblemStatusNum(Json::Value &updatejson);

    // 获取题目的测试数量
    int getProblemJudgeNum(std::string id);

    // 获取题目标题
    std::string getProblemTitleById(std::string id);

    // 获取题目时间限制
    int getProblemTimeLimit(std::string id);

    // 获取题目空间限制
    int getProblemMemoryLimit(std::string id);

private:
    ProblemSet();
    ~ProblemSet();

private:
    std::map<std::string, Problem *> heap;
    int ProblemNum;
};

#endif