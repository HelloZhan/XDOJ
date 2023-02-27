#include "ProblemSet.h"
#include "DataBase.h"
#include "Problem.h"

ProblemSet::ProblemSet()
{
}
void ProblemSet::Init()
{
    MyDB::GetInstance().InitDB("192.168.49.130", "root", "1", "XDOJ");
    Json::Value jsonvalue = MyDB::GetInstance().getAllProblemInfo();
    ProblemNum = (int)jsonvalue["Array"].size();
    for (int i = 0; i < ProblemNum; i++)
    {
        Problem *tmp = new Problem();
        tmp->Init(jsonvalue["Array"][i]);
        heap[jsonvalue["Array"][i]["Pid"].asString()] = tmp;
    }
}
std::string ProblemSet::getProblemDescription(std::string id)
{
    return heap[id]->getProblemDescription();
}

// 获取题库数据
Json::Value ProblemSet::getProblemSet()
{
    Json::Value resjson;
    for (auto v : heap)
    {
        resjson.append(v.second->getInfo());
    }
    return resjson;
}

int ProblemSet::getProblemJudgeNum(std::string id)
{
    return heap[id]->getJudgeNum();
}

ProblemSet &ProblemSet::GetInstance()
{
    static ProblemSet problemset;
    return problemset;
}

ProblemSet::~ProblemSet()
{
    for (auto v : heap)
    {
        delete v.second;
    }
    heap.clear();
}