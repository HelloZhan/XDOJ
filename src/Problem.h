#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <jsoncpp/json/json.h>

class Problem
{
public:
    Problem();
    bool Init(Json::Value &jsonvalue);
    Json::Value getInfo();
    int getJudgeNum();
    std::string getProblemDescription();

private:
    std::string m_id;
    std::string m_title;
    std::string m_description;
    int m_judgenum;
    int m_submitnum;
    int m_acceptnum;
};

#endif