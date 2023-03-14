#include "ProblemSet.h"
#include "MongoDataBase.h"
#include "Problem.h"
#include <iostream>
using namespace std;
ProblemSet::ProblemSet()
{
}
void ProblemSet::Init()
{
    Json::Value jsonvalue = MoDB::GetInstance().getAllProblem();
    ProblemNum = (int)jsonvalue.size();
    for (int i = 0; i < ProblemNum; i++)
    {
        Problem *tmp = new Problem(jsonvalue[i]);
        heap[jsonvalue[i]["_id"].asString()] = tmp;
    }
}
std::string ProblemSet::getProblemDescription(std::string id)
{
    return heap[id]->getProblemDescription();
}

Json::Value ProblemSet::SelectProblemSetInfo(Json::Value &queryjson)
{
    return MoDB::GetInstance().getProblemSet(queryjson);
}

bool ProblemSet::UpdateProblemStatusNum(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateProblemStatusNum(updatejson);
}

int ProblemSet::getProblemJudgeNum(std::string id)
{
    return heap[id]->getJudgeNum();
}

string ProblemSet::getProblemTitleById(std::string id)
{
    return heap[id]->getProblemTitle();
}

int ProblemSet::getProblemTimeLimit(std::string id)
{
    return heap[id]->getProblemTimeLimit();
}

int ProblemSet::getProblemMemoryLimit(std::string id)
{
    return heap[id]->getProblemMemoryLimit();
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