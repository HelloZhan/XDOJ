#include "Control.h"
#include "ProblemList.h"
#include "UserList.h"
#include "StatusRecordList.h"
#include "DiscussList.h"
#include "CommentList.h"
#include "SolutionList.h"
#include "AnnouncementList.h"
#include "Tag.h"
#include "Judger.h"
#include <iostream>
using namespace std;

Json::Value Control::RegisterUser(Json::Value &registerjson)
{
	return UserList::GetInstance()->RegisterUser(registerjson);
}

Json::Value Control::LoginUser(Json::Value &loginjson)
{
	return UserList::GetInstance()->LoginUser(loginjson);
}

Json::Value Control::SelectUserRank(Json::Value &queryjson)
{
	return UserList::GetInstance()->SelectUserRank(queryjson);
}

Json::Value Control::SelectUserInfo(Json::Value &queryjson)
{
	return UserList::GetInstance()->SelectUserInfo(queryjson);
}

Json::Value Control::UpdateUserInfo(Json::Value &updatejson)
{
	return UserList::GetInstance()->UpdateUserInfo(updatejson);
}

Json::Value Control::SelectUserUpdateInfo(Json::Value &queryjson)
{
	return UserList::GetInstance()->SelectUserUpdateInfo(queryjson);
}

Json::Value Control::SelectUserSetInfo(Json::Value &queryjson)
{
	return UserList::GetInstance()->SelectUserSetInfo(queryjson);
}

Json::Value Control::DeleteUser(Json::Value &deletejson)
{
	return UserList::GetInstance()->DeleteUser(deletejson);
}

Json::Value Control::SelectProblemInfoByAdmin(Json::Value &queryjson)
{
	return ProblemList::GetInstance()->SelectProblemInfoByAdmin(queryjson);
}

Json::Value Control::SelectProblem(Json::Value &queryjson)
{
	return ProblemList::GetInstance()->SelectProblem(queryjson);
}

Json::Value Control::EditProblem(Json::Value &insertjson)
{
	if (insertjson["EditType"].asString() == "Insert")
	{
		return ProblemList::GetInstance()->InsertProblem(insertjson);
	}
	else if (insertjson["EditType"].asString() == "Update")
	{
		return ProblemList::GetInstance()->UpdateProblem(insertjson);
	}
}
Json::Value Control::DeleteProblem(Json::Value &deletejson)
{
	return ProblemList::GetInstance()->DeleteProblem(deletejson);
}

Json::Value Control::SelectProblemList(Json::Value &queryjson)
{
	return ProblemList::GetInstance()->SelectProblemList(queryjson);
}

Json::Value Control::SelectProblemListByAdmin(Json::Value &queryjson)
{
	return ProblemList::GetInstance()->SelectProblemListByAdmin(queryjson);
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

	string submitid = StatusRecordList::GetInstance()->InsertStatusRecord(insertjson);

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
	Json::Value statusjson = StatusRecordList::GetInstance()->UpdateStatusRecord(resjson);

	// 更新题目的状态
	Json::Value updatejson;
	updatejson["ProblemId"] = judgejson["ProblemId"];
	updatejson["Status"] = statusjson["Status"];
	ProblemList::GetInstance()->UpdateProblemStatusNum(updatejson);

	updatejson["UserId"] = judgejson["UserId"];

	// 更新用户的状态
	if (UserList::GetInstance()->UpdateUserProblemInfo(updatejson))
		statusjson["IsFirstAC"] = true;
	else
		statusjson["IsFirstAC"] = false;

	return statusjson;
}

Json::Value Control::SelectStatusRecordList(Json::Value &queryjson)
{
	return StatusRecordList::GetInstance()->SelectStatusRecordList(queryjson);
}

Json::Value Control::SelectStatusRecord(Json::Value &queryjson)
{
	return StatusRecordList::GetInstance()->SelectStatusRecord(queryjson);
}

Json::Value Control::GetComment(Json::Value &queryjson)
{
	if (queryjson["Type"].asString() == "Father")
	{
		return CommentList::GetInstance()->getFatherComment(queryjson);
	}
	else
	{
		return CommentList::GetInstance()->getSonComment(queryjson);
	}
}

Json::Value Control::InsertComment(Json::Value &insertjson)
{
	// 文章添加评论数
	Json::Value updatejson;
	updatejson["ArticleId"] = insertjson["ArticleId"];
	updatejson["Num"] = 1;
	if (insertjson["ArticleType"].asString() == "Discuss")
	{
		DiscussList::GetInstance()->UpdateDiscussComments(updatejson);
	}
	else if (insertjson["ArticleType"].asString() == "Solution")
	{
		SolutionList::GetInstance()->UpdateSolutionComments(updatejson);
	}
	else if (insertjson["ArticleType"].asString() == "Announcement")
	{
		AnnouncementList::GetInstance()->UpdateAnnouncementComments(updatejson);
	}

	if (insertjson["Type"].asString() == "Father") // 父评论
	{

		return CommentList::GetInstance()->InsertFatherComment(insertjson);
	}
	else // 子评论
	{
		return CommentList::GetInstance()->InsertSonComment(insertjson);
	}
}

// Json(ArticleId,ArticleType,CommentId)
Json::Value Control::DeleteComment(Json::Value &deletejson)
{
	string articleid = deletejson["ArticleId"].asString();
	string articletype = deletejson["ArticleType"].asString();

	Json::Value resjson;
	// 删除父评论
	Json::Value json = CommentList::GetInstance()->DeleteFatherComment(deletejson);
	// 如果失败删除子评论
	if (json["Result"] == "Fail")
		json = CommentList::GetInstance()->DeleteSonComment(deletejson);
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
	articletype = json["ArticleType"].asString();

	if (articletype == "Discuss")
	{
		DiscussList::GetInstance()->UpdateDiscussComments(articlejson);
	}
	else if (articletype == "Solution")
	{
		SolutionList::GetInstance()->UpdateSolutionComments(articlejson);
	}
	else if (articletype == "Announcement")
	{
		AnnouncementList::GetInstance()->UpdateAnnouncementComments(articlejson);
	}

	resjson["Result"] = "Success";
	return resjson;
}
// --------------------公告------------------------
Json::Value Control::SelectAnnouncementList(Json::Value &queryjson)
{
	return AnnouncementList::GetInstance()->SelectAnnouncementList(queryjson);
}

Json::Value Control::SelectAnnouncement(Json::Value &queryjson)
{
	return AnnouncementList::GetInstance()->SelectAnnouncement(queryjson);
}

Json::Value Control::SelectAnnouncementByEdit(Json::Value &queryjson)
{
	return AnnouncementList::GetInstance()->SelectAnnouncementByEdit(queryjson);
}

Json::Value Control::InsertAnnouncement(Json::Value &insertjson)
{
	return AnnouncementList::GetInstance()->InsertAnnouncement(insertjson);
}

Json::Value Control::UpdateAnnouncement(Json::Value &updatejson)
{
	return AnnouncementList::GetInstance()->UpdateAnnouncement(updatejson);
}
Json::Value Control::DeleteAnnouncement(Json::Value &deletejson)
{
	Json::Value resjson = AnnouncementList::GetInstance()->DeleteAnnouncement(deletejson);
	if (resjson["Result"].asString() == "Success")
	{
		Json::Value json;
		json["ArticleId"] = deletejson["AnnouncementId"];
		CommentList::GetInstance()->DeleteArticleComment(json);
	}
	return resjson;
}

// ----------------------题解----------------------------
Json::Value Control::SelectSolutionList(Json::Value &queryjson)
{
	return SolutionList::GetInstance()->SelectSolutionList(queryjson);
}

Json::Value Control::SelectSolutionListByAdmin(Json::Value &queryjson)
{
	return SolutionList::GetInstance()->SelectSolutionListByAdmin(queryjson);
}

Json::Value Control::SelectSolution(Json::Value &queryjson)
{
	return SolutionList::GetInstance()->SelectSolution(queryjson);
}

Json::Value Control::SelectSolutionByEdit(Json::Value &queryjson)
{
	return SolutionList::GetInstance()->SelectSolutionByEdit(queryjson);
}

Json::Value Control::InsertSolution(Json::Value &insertjson)
{
	return SolutionList::GetInstance()->InsertSolution(insertjson);
}

Json::Value Control::UpdateSolution(Json::Value &updatejson)
{
	return SolutionList::GetInstance()->UpdateSolution(updatejson);
}
Json::Value Control::DeleteSolution(Json::Value &deletejson)
{
	Json::Value resjson = SolutionList::GetInstance()->DeleteSolution(deletejson);
	if (resjson["Result"].asString() == "Success")
	{
		Json::Value json;
		json["ArticleId"] = deletejson["SolutionId"];
		CommentList::GetInstance()->DeleteArticleComment(json);
	}
	return resjson;
}

// ----------------------讨论----------------------------
Json::Value Control::SelectDiscussList(Json::Value &queryjson)
{
	return DiscussList::GetInstance()->SelectDiscussList(queryjson);
}

Json::Value Control::SelectDiscussListByAdmin(Json::Value &queryjson)
{
	return DiscussList::GetInstance()->SelectDiscussListByAdmin(queryjson);
}

Json::Value Control::SelectDiscuss(Json::Value &queryjson)
{
	return DiscussList::GetInstance()->SelectDiscuss(queryjson);
}

Json::Value Control::SelectDiscussByEdit(Json::Value &queryjson)
{
	return DiscussList::GetInstance()->SelectDiscussByEdit(queryjson);
}

Json::Value Control::InsertDiscuss(Json::Value &insertjson)
{
	return DiscussList::GetInstance()->InsertDiscuss(insertjson);
}

Json::Value Control::UpdateDiscuss(Json::Value &updatejson)
{
	return DiscussList::GetInstance()->UpdateDiscuss(updatejson);
}
Json::Value Control::DeleteDiscuss(Json::Value &deletejson)
{
	Json::Value resjson = DiscussList::GetInstance()->DeleteDiscuss(deletejson);
	if (resjson["Result"].asString() == "Success")
	{
		Json::Value json;
		json["ArticleId"] = deletejson["DiscussId"];
		CommentList::GetInstance()->DeleteArticleComment(json);
	}
	return resjson;
}

Json::Value Control::GetTags(Json::Value &queryjson)
{
	if (queryjson["TagType"].asString() == "Problem")
	{
		return Tag::GetInstance()->getProblemTags();
	}
}

Control::Control()
{
	// 初始化题目标签
	Tag::GetInstance()->InitProblemTags();
}

Control::~Control()
{
}