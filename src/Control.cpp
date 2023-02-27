#include "DataBase.h"
#include "Control.h"
#include "ProblemSet.h"

using namespace std;
string Control::GetProblemDescription(string problemid)
{
    return ProblemSet::GetInstance().getProblemDescription(problemid);
}

Json::Value Control::GetProblemSet()
{
    return ProblemSet::GetInstance().getProblemSet();
}

Json::Value Control::GetJudgeCode(Json::Value judgejson)
{
    Json::Value runjson;
    runjson["code"] = judgejson["code"];
    runjson["runid"] = "100";
    runjson["problemid"] = judgejson["id"];
    runjson["language"] = "c_cpp";
    runjson["judgenum"] = ProblemSet::GetInstance().getProblemJudgeNum(judgejson["id"].asString());
    runjson["timelimit"] = 2000;
    runjson["memorylimit"] = 134217728;

    return judger.Run(runjson);
}
Control::Control()
{
    // 连接MySQL数据库
    MyDB::GetInstance().InitDB("192.168.49.130", "root", "1", "XDOJ");

    // 初始化题库
    ProblemSet::GetInstance().Init();
}

Control::~Control()
{
}