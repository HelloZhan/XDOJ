#include "MongoDataBase.h"
#include "Control.h"
#include "ProblemSet.h"
#include "UserSet.h"
#include "StatusRecord.h"
#include "Discuss.h"
#include "Comment.h"
#include <iostream>
using namespace std;

Json::Value Control::RegisterUser(Json::Value &registerjson)
{
	return UserSet::GetInstance().RegisterUser(registerjson);
}

Json::Value Control::LoginUser(Json::Value &loginjson)
{
	return UserSet::GetInstance().LoginUser(loginjson);
}

Json::Value Control::SelectUserRank(Json::Value &queryjson)
{
	return UserSet::GetInstance().SelectUserRank(queryjson);
}

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
	string problemid = judgejson["ProblemId"].asString();

	// 添加状态记录
	// 传入：Json(ProblemId,UserId,UserNickName,ProblemTitle,Language,Code);
	Json::Value insertjson;
	insertjson["ProblemId"] = problemid;
	insertjson["UserId"] = judgejson["UserId"];
	insertjson["UserNickName"] = judgejson["UserNickName"];
	insertjson["ProblemTitle"] = ProblemSet::GetInstance().getProblemTitleById(problemid);
	insertjson["Language"] = judgejson["Language"];
	insertjson["Code"] = judgejson["Code"];

	string submitid = StatusRecord::GetInstance().InsertStatusRecord(insertjson);

	// 运行代码
	// Json(SubmitId,ProblemId,JudgeNum,Code,Language,TimeLimit,MemoryLimit)
	Json::Value runjson;
	runjson["Code"] = judgejson["Code"];
	runjson["SubmitId"] = submitid;
	runjson["ProblemId"] = problemid;
	runjson["Language"] = judgejson["Language"];
	runjson["JudgeNum"] = ProblemSet::GetInstance().getProblemJudgeNum(problemid);
	runjson["TimeLimit"] = ProblemSet::GetInstance().getProblemTimeLimit(problemid);
	runjson["MemoryLimit"] = ProblemSet::GetInstance().getProblemMemoryLimit(problemid);

	Json::Value resjson = judger.Run(runjson);

	// 更新状态信息 返回结果
	/*
		传入：Json(SubmitId,Status,RunTime,RunMemory,Length,ComplierInfo,
		TestInfo[(Status,StandardOutput,PersonalOutput,RunTime,RunMemory)])
	*/
	Json::Value statusjson = StatusRecord::GetInstance().UpdateStatusRecord(resjson);

	// 更新题目的状态
	Json::Value updatejson;
	updatejson["ProblemId"] = problemid;
	updatejson["Status"] = statusjson["Status"].asString();
	ProblemSet::GetInstance().UpdateProblemStatusNum(updatejson);

	updatejson["UserId"] = judgejson["UserId"];

	// 更新用户的状态
	if (UserSet::GetInstance().UpdateUserProblemInfo(updatejson))
		statusjson["IsFirstAC"] = true;
	else
		statusjson["IsFirstAC"] = false;

	return statusjson;
}

Json::Value Control::SelectStatusRecordInfo(Json::Value &queryjson)
{
	return StatusRecord::GetInstance().SelectStatusRecordInfo(queryjson);
}

Json::Value Control::GetAllDiscuss()
{
	return Disscuss::GetInstance().getAllDiscuss();
}

Json::Value Control::SelectDiscuss(Json::Value &queryjson)
{
	return Disscuss::GetInstance().SelectDiscuss(queryjson);
}

Json::Value Control::SelectDiscussContent(Json::Value &queryjson)
{
	return Disscuss::GetInstance().SelectDiscussContent(queryjson);
}

Json::Value Control::GetComment(Json::Value &queryjson)
{
	if (queryjson["Type"].asString() == "Father")
	{
		return Comment::GetInstance().getFatherComment(queryjson);
	}
	else
	{
		return Comment::GetInstance().getSonComment(queryjson);
	}
}

Json::Value Control::InsertComment(Json::Value &insertjson)
{
	string type = insertjson["Type"].asString();
	if (type == "Father")
	{
		return Comment::GetInstance().InsertFatherComment(insertjson);
	}
	else
	{
		return Comment::GetInstance().InsertSonComment(insertjson);
	}
}

Control::Control()
{
	// 初始化MongoDB数据库
	MoDB::GetInstance().InitDB();

	// 初始化题库
	ProblemSet::GetInstance().Init();
}

Control::~Control()
{
}