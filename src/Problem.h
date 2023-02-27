#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <jsoncpp/json/json.h>

class Problem
{
public:
    Problem(Json::Value &jsonvalue);

    // 获取信息
    Json::Value getInfo();

    // 获取该题的判题数量
    int getJudgeNum();

    // 获取该题的详细描述
    std::string getProblemDescription();

private:
    std::string m_problemid;
    std::string m_title;
    std::string m_description;
    int m_judgenum;
    int m_submitnum;
    int m_cenum;
    int m_acnum;
    int m_wanum;
    int m_tlenum;
    int m_mlenum;
};

#endif