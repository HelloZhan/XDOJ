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

Json::Value Control::SelectProblemInfoByAdmin(Json::Value &queryjson)
{
	return ProblemSet::GetInstance().SelectProblemInfoByAdmin(queryjson);
}

Json::Value Control::InsertProblem(Json::Value &insertjson)
{
	return ProblemSet::GetInstance().InsertProblem(insertjson);
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

	Json::Value updatejson;
	updatejson["ArticleId"] = insertjson["ArticleId"];
	updatejson["Num"] = 1;
	// 文章添加评论数
	if (insertjson["ArticleType"].asString() == "Discuss")
	{
		Discuss::GetInstance().UpdateDiscussComments(updatejson);
	}

	if (type == "Father") // 父评论
	{

		return Comment::GetInstance().InsertFatherComment(insertjson);
	}
	else // 子评论
	{
		return Comment::GetInstance().InsertSonComment(insertjson);
	}
}

// Json(ArticleId,ArticleType,CommentId)
Json::Value Control::DeleteComment(Json::Value &deletejson)
{
	string articleid = deletejson["ArticleId"].asString();
	string articletype = deletejson["ArticleType"].asString();

	Json::Value resjson;
	// 删除父评论
	Json::Value json = Comment::GetInstance().DeleteFatherComment(deletejson);
	// 如果失败删除子评论
	if (json["Result"] == "Fail")
		json = Comment::GetInstance().DeleteSonComment(deletejson);
	// 如果都失败，返回失败结果
	if (json["Result"] == "Fail")
	{
		resjson["Result"] = "Fail";
		resjson["Reason"] = "数据库未查询到数据！";
		return resjson;
	}
	// 如果删除评论成功，更新文章的评论数量
	Json::Value articlejson;
	articlejson["Num"] = stoi(json["DeleteNum"].asString()) * -1;
	articlejson["ArticleId"] = articleid;

	if (articletype == "Discuss")
	{
		Discuss::GetInstance().UpdateDiscussComments(articlejson);
	}

	resjson["Result"] = "Success";
	return resjson;
}

Json::Value Control::SelectArticle(Json::Value &queryjson)
{
	if (queryjson["ArticleType"].asString() == "Discuss")
	{
		return Discuss::GetInstance().SelectDiscuss(queryjson);
	}
}

Json::Value Control::SelectArticleContent(Json::Value &queryjson)
{
	if (queryjson["ArticleType"].asString() == "Discuss")
	{
		return Discuss::GetInstance().SelectDiscussContent(queryjson);
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
	// 删除文章所有评论
	Comment::GetInstance().DeleteArticleComment(deletejson);

	if (deletejson["ArticleType"].asString() == "Discuss")
	{
		return Discuss::GetInstance().DeleteDiscuss(deletejson);
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