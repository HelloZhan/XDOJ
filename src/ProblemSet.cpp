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
        heap[jsonvalue["Array"][i]["ProblemId"].asString()] = tmp;
    }
}
std::string ProblemSet::getProblemDescription(std::string id)
{
    return heap[id]->getProblemDescription();
}

Json::Value ProblemSet::SelectProblemSetInfo(Json::Value &queryjson)
{
    return MyDB::GetInstance().SelectProblemSetInfo(queryjson);
}

int ProblemSet::getProblemJudgeNum(std::string id)
{
    return heap[id]->getJudgeNum();
}

string ProblemSet::getProblemTitleById(std::string id)
{
    return heap[id]->getProblemTitle();
}

bool ProblemSet::UpdateProblemStatusNumById(string &id, string &status)
{
    string statusnum;
    if (status == CE)
        statusnum = "CENum";
    else if (status == AC)
        statusnum = "ACNum";
    else if (status == WA)
        statusnum = "WANum";
    else if (status == TLE)
        statusnum = "TLENum";
    else if (status == MLE)
        statusnum = "MLENum";

    return heap[id]->UpdateStatusNum(statusnum);
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