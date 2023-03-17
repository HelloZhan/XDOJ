#include "ProblemSet.h"
#include "MongoDataBase.h"
#include "Problem.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

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

Json::Value ProblemSet::SelectProblemInfoByAdmin(Json::Value &queryjson)
{
    // 获取基本信息
    Json::Value resjson = MoDB::GetInstance().SelectProblemInfoByAdmin(queryjson);

    // 获取测试点信息
    string problemid = resjson["_id"].asString();
    int judgenum = stoi(resjson["JudgeNum"].asString());
    string DATA_PATH = "../../problemdata/" + problemid + "/";
    ifstream infilein, infileout;
    for (int i = 1; i <= judgenum; i++)
    {
        Json::Value jsoninfo;
        string infilepath = DATA_PATH + to_string(i) + ".in";
        string outfilepath = DATA_PATH + to_string(1) + ".out";

        infilein.open(infilepath.data());
        string infile((istreambuf_iterator<char>(infilein)),
                      (istreambuf_iterator<char>()));

        infileout.open(outfilepath.data());
        string outfile((istreambuf_iterator<char>(infileout)),
                       (istreambuf_iterator<char>()));
        jsoninfo["in"] = infile;
        jsoninfo["out"] = outfile;

        infilein.close();
        infileout.close();

        resjson["TestInfo"].append(jsoninfo);
    }
    // 获取SPJ文件
    string spjpath = DATA_PATH + "spj.cpp";
    if (access(spjpath.data(), F_OK) == 0)
    {
        ifstream infilespj;
        infilespj.open(spjpath.data());
        string spjfile((istreambuf_iterator<char>(infilespj)),
                       (istreambuf_iterator<char>()));
        resjson["SPJ"] = spjfile;
        infilespj.close();
    }
    return resjson;
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