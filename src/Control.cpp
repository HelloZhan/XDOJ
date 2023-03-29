#include "Control.h"
#include "ProblemList.h"
#include "UserSet.h"
#include "StatusRecordList.h"
#include "Discuss.h"
#include "Comment.h"
#include "Solution.h"
#include "Announcement.h"
#include "Tag.h"
#include "Judger.h"
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

Json::Value Control::SelectUserUpdateInfo(Json::Value &queryjson)
{
	return UserSet::GetInstance().SelectUserUpdateInfo(queryjson);
}

Json::Value Control::SelectUserSetInfo(Json::Value &queryjson)
{
	return UserSet::GetInstance().SelectUserSetInfo(queryjson);
}

Json::Value Control::DeleteUser(Json::Value &deletejson)
{
	return UserSet::GetInstance().DeleteUser(deletejson);
}

Json::Value Control::SelectProblemInfoByAdmin(Json::Value &queryjson)
{
	return ProblemList::GetInstance().SelectProblemInfoByAdmin(queryjson);
}

Json::Value Control::SelectProblem(Json::Value &queryjson)
{
	return ProblemList::GetInstance().SelectProblem(queryjson);
}

Json::Value Control::EditProblem(Json::Value &insertjson)
{
	if (insertjson["EditType"].asString() == "Insert")
	{
		return ProblemList::GetInstance().InsertProblem(insertjson);
	}
	else if (insertjson["EditType"].asString() == "Update")
	{
		return ProblemList::GetInstance().UpdateProblem(insertjson);
	}
}
Json::Value Control::DeleteProblem(Json::Value &deletejson)
{
	return ProblemList::GetInstance().DeleteProblem(deletejson);
}

Json::Value Control::SelectProblemList(Json::Value &queryjson)
{
	return ProblemList::GetInstance().SelectProblemList(queryjson);
}

Json::Value Control::GetJudgeCode(Json::Value judgejson)
{
	// 传入Json(ProblemId,UserId,UserNickName,Code,Language,TimeLimit,MemoryLimit,JudgeNum,ProblemTitle)

	// 添加状态记录
	// 传入：Json(ProblemId,UserId,UserNickName,ProblemTitle,Language,Code);
	Json::Value insertjson;
	insertjson["ProblemId"] = judgejson["ProblemId"];
	insertjson["UserId"] = judgejson["UserId"];
	insertjson["UserNickName"] = judgejson["UserNickName"];
	insertjson["ProblemTitle"] = judgejson["ProblemTitle"];
	insertjson["Language"] = judgejson["Language"];
	insertjson["Code"] = judgejson["Code"];

	string submitid = StatusRecordList::GetInstance().InsertStatusRecord(insertjson);

	// 运行代码
	// Json(SubmitId,ProblemId,JudgeNum,Code,Language,TimeLimit,MemoryLimit)
	Json::Value runjson;
	runjson["Code"] = judgejson["Code"];
	runjson["SubmitId"] = submitid;
	runjson["ProblemId"] = judgejson["ProblemId"];
	runjson["Language"] = judgejson["Language"];
	runjson["JudgeNum"] = judgejson["JudgeNum"];
	runjson["TimeLimit"] = judgejson["TimeLimit"];
	runjson["MemoryLimit"] = judgejson["MemoryLimit"];

	// 创建判题对象
	Judger judger;
	Json::Value resjson = judger.Run(runjson);

	// 更新状态信息 返回结果
	/*
		传入：Json(SubmitId,Status,RunTime,RunMemory,Length,ComplierInfo,
		TestInfo[(Status,StandardOutput,PersonalOutput,RunTime,RunMemory)])
	*/
	Json::Value statusjson = StatusRecordList::GetInstance().UpdateStatusRecord(resjson);

	// 更新题目的状态
	Json::Value updatejson;
	updatejson["ProblemId"] = judgejson["ProblemId"];
	updatejson["Status"] = statusjson["Status"];
	ProblemList::GetInstance().UpdateProblemStatusNum(updatejson);

	updatejson["UserId"] = judgejson["UserId"];

	// 更新用户的状态
	if (UserSet::GetInstance().UpdateUserProblemInfo(updatejson))
		statusjson["IsFirstAC"] = true;
	else
		statusjson["IsFirstAC"] = false;

	return statusjson;
}

Json::Value Control::SelectStatusRecordList(Json::Value &queryjson)
{
	return StatusRecordList::GetInstance().SelectStatusRecordList(queryjson);
}

Json::Value Control::SelectStatusRecord(Json::Value &queryjson)
{
	return StatusRecordList::GetInstance().SelectStatusRecord(queryjson);
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
	else if (insertjson["ArticleType"].asString() == "Solution")
	{
		Solution::GetInstance().UpdateSolutionComments(updatejson);
	}
	else if (insertjson["ArticleType"].asString() == "Announcement")
	{
		Announcement::GetInstance().UpdateAnnouncementComments(updatejson);
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
// --------------------公告------------------------
Json::Value Control::SelectAnnouncement(Json::Value &queryjson)
{
	return Announcement::GetInstance().SelectAnnouncement(queryjson);
}

Json::Value Control::SelectAnnouncementContent(Json::Value &queryjson)
{
	return Announcement::GetInstance().SelectAnnouncementContent(queryjson);
}

Json::Value Control::SelectAnnouncementByEdit(Json::Value &queryjson)
{
	return Announcement::GetInstance().SelectAnnouncementByEdit(queryjson);
}

Json::Value Control::InsertAnnouncement(Json::Value &insertjson)
{
	return Announcement::GetInstance().InsertAnnouncement(insertjson);
}

Json::Value Control::UpdateAnnouncement(Json::Value &updatejson)
{
	return Announcement::GetInstance().UpdateAnnouncement(updatejson);
}
Json::Value Control::DeleteAnnouncement(Json::Value &deletejson)
{
	return Announcement::GetInstance().DeleteAnnouncement(deletejson);
}

// ----------------------题解----------------------------
Json::Value Control::SelectSolution(Json::Value &queryjson)
{
	return Solution::GetInstance().SelectSolution(queryjson);
}

Json::Value Control::SelectSolutionByAdmin(Json::Value &queryjson)
{
	return Solution::GetInstance().SelectSolutionByAdmin(queryjson);
}

Json::Value Control::SelectSolutionContent(Json::Value &queryjson)
{
	return Solution::GetInstance().SelectSolutionContent(queryjson);
}

Json::Value Control::SelectSolutionByEdit(Json::Value &queryjson)
{
	return Solution::GetInstance().SelectSolutionByEdit(queryjson);
}

Json::Value Control::InsertSolution(Json::Value &insertjson)
{
	return Solution::GetInstance().InsertSolution(insertjson);
}

Json::Value Control::UpdateSolution(Json::Value &updatejson)
{
	return Solution::GetInstance().UpdateSolution(updatejson);
}
Json::Value Control::DeleteSolution(Json::Value &deletejson)
{
	return Solution::GetInstance().DeleteSolution(deletejson);
}

// ----------------------讨论----------------------------
Json::Value Control::SelectDiscuss(Json::Value &queryjson)
{
	return Discuss::GetInstance().SelectDiscuss(queryjson);
}

Json::Value Control::SelectDiscussByAdmin(Json::Value &queryjson)
{
	return Discuss::GetInstance().SelectDiscussByAdmin(queryjson);
}

Json::Value Control::SelectDiscussContent(Json::Value &queryjson)
{
	return Discuss::GetInstance().SelectDiscussContent(queryjson);
}

Json::Value Control::SelectDiscussByEdit(Json::Value &queryjson)
{
	return Discuss::GetInstance().SelectDiscussByEdit(queryjson);
}

Json::Value Control::InsertDiscuss(Json::Value &insertjson)
{
	return Discuss::GetInstance().InsertDiscuss(insertjson);
}

Json::Value Control::UpdateDiscuss(Json::Value &updatejson)
{
	return Discuss::GetInstance().UpdateDiscuss(updatejson);
}
Json::Value Control::DeleteDiscuss(Json::Value &deletejson)
{
	return Discuss::GetInstance().DeleteDiscuss(deletejson);
}

Json::Value Control::GetTags(Json::Value &queryjson)
{
	if (queryjson["TagType"].asString() == "Problem")
	{
		return Tag::GetInstance().getProblemTags();
	}
}

Control::Control()
{
	// 初始化题目标签
	Tag::GetInstance().InitProblemTags();
}

Control::~Control()
{
}