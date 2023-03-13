#include "Problem.h"
#include <iostream>
using namespace std;
Problem::Problem(Json::Value &jsonvalue)
{
    m_problemid = jsonvalue["_id"].asString();
    m_title = jsonvalue["Title"].asString();
    m_description = jsonvalue["Description"].asString();
    m_judgenum = stoi(jsonvalue["JudgeNum"].asString());
    m_timelimit = stoi(jsonvalue["TimeLimit"].asString());
    m_memorylimit = stoi(jsonvalue["MemoryLimit"].asString());
}

int Problem::getJudgeNum()
{
    return m_judgenum;
}
std::string Problem::getProblemDescription()
{
    return m_description;
}

std::string Problem::getProblemTitle()
{
    return m_title;
}

int Problem::getProblemTimeLimit()
{
    return m_timelimit;
}

int Problem::getProblemMemoryLimit()
{
    return m_memorylimit;
}
