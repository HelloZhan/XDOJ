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
    ProblemSet::GetInstance().Init();
}

Control::~Control()
{
}