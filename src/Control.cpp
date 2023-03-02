#include "DataBase.h"
#include "Control.h"
#include "ProblemSet.h"
#include "UserSet.h"
#include "StatusRecord.h"
#include <iostream>
using namespace std;
string Control::GetProblemDescription(string problemid)
{
    return ProblemSet::GetInstance().getProblemDescription(problemid);
}

Json::Value Control::SelectProblemSetInfo(Json::Value &queryjson)
{
    return ProblemSet::GetInstance().SelectProblemSetInfo(queryjson);
}

Json::Value Control::GetJudgeCode(Json::Value judgejson)
{
    // 传入：Json(ProblemId,UserId,UserNickName,ProblemTitle,Language,Code);
    Json::Value insertjson;
    insertjson["ProblemId"] = judgejson["ProblemId"];
    insertjson["UserId"] = judgejson["UserId"];
    insertjson["UserNickName"] = UserSet::GetInstance().getUserNickNameById(judgejson["UserId"].asString());
    insertjson["ProblemTitle"] = ProblemSet::GetInstance().getProblemTitleById(judgejson["ProblemId"].asString());
    insertjson["Language"] = judgejson["Language"];
    insertjson["Code"] = judgejson["Code"];
    cout << "judgejson" << endl;
    cout << judgejson.toStyledString() << endl;
    string submitid = MyDB::GetInstance().InsertStatusRecordInfo(insertjson);

    Json::Value runjson;
    runjson["Code"] = judgejson["Code"];
    runjson["SubmitId"] = submitid;
    runjson["ProblemId"] = judgejson["ProblemId"];
    runjson["Language"] = judgejson["Language"];
    runjson["JudgeNum"] = ProblemSet::GetInstance().getProblemJudgeNum(judgejson["ProblemId"].asString());
    runjson["TimeLimit"] = 2000;
    runjson["MemoryLimit"] = 134217728;

    Json::Value resjson = judger.Run(runjson);
    // 传入：Json(SubmitId,Status,RunTime,RunMemory,Length)
    Json::Value updatejson;
    updatejson["SubmitId"] = submitid;
    updatejson["Status"] = resjson["Result"];
    updatejson["RunTime"] = resjson["RunTime"];
    updatejson["RunMemory"] = resjson["RunMemory"];
    updatejson["Length"] = resjson["Length"];
    MyDB::GetInstance().UpdateStatusRecordInfo(updatejson);
    return resjson;
}

Json::Value Control::SelectStatusRecordInfo(Json::Value &queryjson)
{
    return StatusRecord::GetInstance().SelectStatusRecordInfo(queryjson);
}

Control::Control()
{
    // 连接MySQL数据库
    MyDB::GetInstance().InitDB("192.168.49.130", "root", "1", "XDOJ");

    // 初始化题库
    ProblemSet::GetInstance().Init();

    // 初始化用户
    UserSet::GetInstance().LoadDataBaseInfo();
}

Control::~Control()
{
}