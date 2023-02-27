#include "ProblemSet.h"
#include "DataBase.h"
#include "Problem.h"
#include <iostream>
using namespace std;
ProblemSet::ProblemSet()
{
}
void ProblemSet::Init()
{
    Json::Value jsonvalue = MyDB::GetInstance().getAllProblemInfo();
    ProblemNum = (int)jsonvalue["Array"].size();
    for (int i = 0; i < ProblemNum; i++)
    {
        Problem *tmp = new Problem(jsonvalue["Array"][i]);
        cout << tmp->getInfo().toStyledString() << endl;
        heap[jsonvalue["Array"][i]["ProblemId"].asString()] = tmp;
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