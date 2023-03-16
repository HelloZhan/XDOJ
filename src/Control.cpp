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

Json::Value Control::SelectUserInfo(Json::Value &queryjson)
{
	return UserSet::GetInstance().SelectUserInfo(queryjson);
}

Json::Value Control::UpdateUserInfo(Json::Value &updatejson)
{
	return UserSet::GetInstance().UpdateUserInfo(updatejson);
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
	return Discuss::GetInstance().getAllDiscuss();
}

Json::Value Control::SelectDiscuss(Json::Value &queryjson)
{
	return Discuss::GetInstance().SelectDiscuss(queryjson);
}

Json::Value Control::SelectDiscussContent(Json::Value &queryjson)
{
	return Discuss::GetInstance().SelectDiscussContent(queryjson);
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

	// TODO:添加评论，需要将其讨论或者文章的评论数加一
	if (type == "Father") // 父评论
	{

		return Comment::GetInstance().InsertFatherComment(insertjson);
	}
	else // 子评论
	{
		return Comment::GetInstance().InsertSonComment(insertjson);
	}
}
Json::Value Control::InsertArticle(Json::Value &insertjson)
{
	if (insertjson["ArticleType"].asString() == "Discuss") // 插入讨论
	{
		return Discuss::GetInstance().InsertDiscuss(insertjson);
	}
	else // TODO:插入题解
	{
	}
}

Json::Value Control::UpdateArticle(Json::Value &updatejson)
{
	if (updatejson["ArticleType"].asString() == "Discuss") // 更新讨论
	{
		return Discuss::GetInstance().UpdateDiscuss(updatejson);
	}
	else // TODO：更新题解
	{
	}
}

Json::Value Control::DeleteArticle(Json::Value &deletejson)
{
	if (deletejson["ArticleType"].asString() == "Discuss")
	{
		return Discuss::GetInstance().DeleteDiscuss(deletejson);
	} // TODO:
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