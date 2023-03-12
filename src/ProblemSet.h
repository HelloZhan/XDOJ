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

    std::string getProblemDescription(std::string id);

    // 通过普通查询获取题库数据
    Json::Value SelectProblemSetInfo(Json::Value &queryjson);

    // 获取题目的测试数量
    int getProblemJudgeNum(std::string id);

    // 获取题目标题
    std::string getProblemTitleById(std::string id);

    // 改变题目的状态数量
    bool UpdateProblemStatusNumById(std::string &id, std::string &status);

private:
    ProblemSet();
    ~ProblemSet();

private:
    std::map<std::string, Problem *> heap;
    int ProblemNum;
};

#endif