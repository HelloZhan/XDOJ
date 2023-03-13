#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <jsoncpp/json/json.h>

class Problem
{
public:
    Problem(Json::Value &jsonvalue);

    // 获取该题的判题数量
    int getJudgeNum();

    // 获取该题的详细描述
    std::string getProblemDescription();

    // 获取题目标题
    std::string getProblemTitle();

    // 获取时间限制MS
    int getProblemTimeLimit();

    // 获取空间限制MB
    int getProblemMemoryLimit();

private:
    std::string m_problemid;
    std::string m_title;
    std::string m_description;
    int m_timelimit;   // MS
    int m_memorylimit; // MB
    int m_judgenum;
};

#endif