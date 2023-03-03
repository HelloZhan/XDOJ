#include "Problem.h"
#include "MySQLDataBase.h"

Problem::Problem(Json::Value &jsonvalue)
{
    m_problemid = jsonvalue["ProblemId"].asString();
    m_title = jsonvalue["Title"].asString();
    m_description = jsonvalue["Description"].asString();
    m_judgenum = stoi(jsonvalue["JudgeNum"].asString());
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

bool Problem::UpdateStatusNum(std::string &statusnum)
{
    return MyDB::GetInstance().UpdateProblemStatusNum(m_problemid, statusnum);
}