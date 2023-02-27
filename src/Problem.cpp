#include "Problem.h"

Problem::Problem(Json::Value &jsonvalue)
{
    m_problemid = jsonvalue["ProblemId"].asString();
    m_title = jsonvalue["Title"].asString();
    m_description = jsonvalue["Description"].asString();
    m_judgenum = stoi(jsonvalue["JudgeNum"].asString());
    m_submitnum = stoi(jsonvalue["SubmitNum"].asString());
    m_cenum = stoi(jsonvalue["CENum"].asString());
    m_acnum = stoi(jsonvalue["ACNum"].asString());
    m_wanum = stoi(jsonvalue["WANum"].asString());
    m_tlenum = stoi(jsonvalue["TLENum"].asString());
    m_mlenum = stoi(jsonvalue["MLENum"].asString());
}

Json::Value Problem::getInfo()
{
    Json::Value resjson;
    resjson["ProblemId"] = m_problemid;
    resjson["Title"] = m_title;
    resjson["SubmitNum"] = m_submitnum;
    resjson["CENum"] = m_cenum;
    resjson["ACNum"] = m_acnum;
    resjson["WANum"] = m_wanum;
    resjson["TLENum"] = m_tlenum;
    resjson["MLENum"] = m_mlenum;
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