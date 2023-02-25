#include "Problem.h"

Problem::Problem()
{
}
bool Problem::Init(Json::Value &jsonvalue)
{
    m_id = jsonvalue["Pid"].asString();
    m_title = jsonvalue["Title"].asString();
    m_description = jsonvalue["Description"].asString();
    m_judgenum = stoi(jsonvalue["JudgeNum"].asString());
    m_submitnum = stoi(jsonvalue["SubmitNum"].asString());
    m_acceptnum = stoi(jsonvalue["AcceptNum"].asString());
}
Json::Value Problem::getInfo()
{
    Json::Value resjson;
    resjson["id"] = m_id;
    resjson["title"] = m_title;
    resjson["submitnum"] = m_submitnum;
    resjson["acceptnum"] = m_acceptnum;
    return resjson;
}
int Problem::getJudgeNum()
{
    return m_judgenum;
}
std::string Problem::getProblemDescription()
{
    return m_description;
}