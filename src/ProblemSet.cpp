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
        string outfilepath = DATA_PATH + to_string(i) + ".out";

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
    resjson["IsSPJ"] = false;
    string spjpath = DATA_PATH + "spj.cpp";
    if (access(spjpath.data(), F_OK) == 0)
    {
        ifstream infilespj;
        infilespj.open(spjpath.data());
        string spjfile((istreambuf_iterator<char>(infilespj)),
                       (istreambuf_iterator<char>()));
        resjson["SPJ"] = spjfile;
        resjson["IsSPJ"] = true;
        infilespj.close();
    }
    return resjson;
}
bool InsertProblemDataInfo(Json::Value &insertjson)
{
    // 添加测试用例
    string DATA_PATH = "../../problemdata/" + insertjson["ProblemId"].asString();
    string command = "mkdir " + DATA_PATH;
    // 创建文件夹
    system(command.data());
    // 添加测试文件
    for (int i = 1; i <= insertjson["TestInfo"].size(); i++)
    {
        string index = to_string(i);
        ofstream outfilein, outfileout;
        string inpath = DATA_PATH + "/" + index + ".in";
        string outpath = DATA_PATH + "/" + index + ".out";

        outfilein.open(inpath.data());
        outfilein << insertjson["TestInfo"][i - 1]["in"].asString();
        outfilein.close();

        outfileout.open(outpath.data());
        outfileout << insertjson["TestInfo"][i - 1]["out"].asString();
        outfileout.close();
    }
    // 添加SPJ文件
    if (insertjson["IsSPJ"].asBool())
    {
        ofstream outfilespj;
        string spjpath = DATA_PATH + "/spj.cpp";
        outfilespj.open(spjpath.data());
        outfilespj << insertjson["SPJ"].asString();
        outfilespj.close();
    }
    return true;
}
Json::Value ProblemSet::InsertProblem(Json::Value &insertjson)
{
    cout << insertjson.toStyledString() << endl;
    Json::Value tmpjson = MoDB::GetInstance().InsertProblem(insertjson);

    if (tmpjson["Result"] == "Fail") // 插入失败
        return tmpjson;

    // 插入信息
    Json::Value problemjson;
    problemjson["_id"] = tmpjson["ProblemId"];
    problemjson["Title"] = insertjson["Title"];
    problemjson["Description"] = insertjson["Description"];
    problemjson["JudgeNum"] = insertjson["JudgeNum"];
    problemjson["TimeLimit"] = insertjson["TimeLimit"];
    problemjson["MemoryLimit"] = insertjson["MemoryLimit"];

    Problem *tmp = new Problem(problemjson);
    heap[problemjson["_id"].asString()] = tmp;

    insertjson["ProblemId"] = tmpjson["ProblemId"];

    InsertProblemDataInfo(insertjson);
    return tmpjson;
}

Json::Value ProblemSet::UpdateProblem(Json::Value &updatejson)
{
    cout << "UpdateProblem" << endl;
    Json::Value tmpjson = MoDB::GetInstance().UpdateProblem(updatejson);
    if (tmpjson["Result"].asString() == "Fail")
        return tmpjson;

    string problemid = updatejson["ProblemId"].asString();
    string DATA_PATH = "../../problemdata/" + problemid;
    // 删除文件夹
    string command = "rm -rf " + DATA_PATH;
    system(command.data());
    // 创建文件夹
    InsertProblemDataInfo(updatejson);
    return tmpjson;
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