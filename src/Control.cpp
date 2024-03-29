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

// 无权限
static Json::Value NoPermission;

Json::Value Control::RegisterUser(Json::Value &registerjson)
{
	return UserList::GetInstance()->RegisterUser(registerjson);
}

Json::Value Control::LoginUser(Json::Value &loginjson)
{
	return UserList::GetInstance()->LoginUser(loginjson);
}

Json::Value Control::LoginUserByToken(Json::Value &loginjson)
{
	return UserList::GetInstance()->LoginUserByToken(loginjson);
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
	// 如果不是本人或者管理员 无权修改
	if (!UserList::GetInstance()->IsAuthor(updatejson))
		return NoPermission;

	return UserList::GetInstance()->UpdateUserInfo(updatejson);
}

Json::Value Control::SelectUserUpdateInfo(Json::Value &queryjson)
{
	// 如果不是本人或者管理员 无权查询
	if (!UserList::GetInstance()->IsAuthor(queryjson))
		return NoPermission;

	return UserList::GetInstance()->SelectUserUpdateInfo(queryjson);
}

Json::Value Control::SelectUserSetInfo(Json::Value &queryjson)
{
	// 如果不是管理员，无权查询
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;

	return UserList::GetInstance()->SelectUserSetInfo(queryjson);
}

Json::Value Control::DeleteUser(Json::Value &deletejson)
{
	// 如果不是管理员，无权删除
	if (!UserList::GetInstance()->IsAdministrator(deletejson))
		return NoPermission;

	return UserList::GetInstance()->DeleteUser(deletejson);
}

Json::Value Control::SelectProblemInfoByAdmin(Json::Value &queryjson)
{
	// 如果不是管理员，无权查看
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;

	return ProblemList::GetInstance()->SelectProblemInfoByAdmin(queryjson);
}

Json::Value Control::SelectProblem(Json::Value &queryjson)
{
	return ProblemList::GetInstance()->SelectProblem(queryjson);
}

Json::Value Control::EditProblem(Json::Value &insertjson)
{
	// 如果不是管理员，无权操作
	if (!UserList::GetInstance()->IsAdministrator(insertjson))
		return NoPermission;

	Json::Value resjson;
	if (insertjson["EditType"].asString() == "Insert")
	{
		resjson = ProblemList::GetInstance()->InsertProblem(insertjson);
	}
	else if (insertjson["EditType"].asString() == "Update")
	{
		resjson = ProblemList::GetInstance()->UpdateProblem(insertjson);
	}
	Tag::GetInstance()->InitProblemTags();
	return resjson;
}
Json::Value Control::DeleteProblem(Json::Value &deletejson)
{
	// 如果不是管理员，无权操作
	if (!UserList::GetInstance()->IsAdministrator(deletejson))
		return NoPermission;

	return ProblemList::GetInstance()->DeleteProblem(deletejson);
}

Json::Value Control::SelectProblemList(Json::Value &queryjson)
{
	return ProblemList::GetInstance()->SelectProblemList(queryjson);
}

Json::Value Control::SelectProblemListByAdmin(Json::Value &queryjson)
{
	// 如果不是管理员，无权查询
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;

	return ProblemList::GetInstance()->SelectProblemListByAdmin(queryjson);
}

Json::Value Control::GetJudgeCode(Json::Value judgejson)
{
	// 如果不是用户，无权操作
	if (!UserList::GetInstance()->IsOrdinaryUser(judgejson))
		return NoPermission;

	Json::Value resjson;
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

	if (submitid == "0")
	{
		resjson["Result"] = "Fail";
		resjson["Reason"] = "系统出错！";
		return resjson;
	}

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
	Json::Value json = judger.Run(runjson);

	// 更新状态信息
	/*
		传入：Json(SubmitId,Status,RunTime,RunMemory,Length,ComplierInfo,
		TestInfo[(Status,StandardOutput,PersonalOutput,RunTime,RunMemory)])
	*/
	StatusRecordList::GetInstance()->UpdateStatusRecord(json);
	// 更新题目的状态
	Json::Value updatejson;
	updatejson["ProblemId"] = judgejson["ProblemId"];
	updatejson["Status"] = json["Status"];
	ProblemList::GetInstance()->UpdateProblemStatusNum(updatejson);

	updatejson["UserId"] = judgejson["UserId"];

	// 更新用户的状态
	if (UserList::GetInstance()->UpdateUserProblemInfo(updatejson))
		resjson["IsFirstAC"] = true;
	else
		resjson["IsFirstAC"] = false;

	resjson["Result"] = "Success";
	resjson["Status"] = json["Status"];
	resjson["ComplierInfo"] = json["ComplierInfo"];
	return resjson;
}

Json::Value Control::SelectStatusRecordList(Json::Value &queryjson)
{
	return StatusRecordList::GetInstance()->SelectStatusRecordList(queryjson);
}

Json::Value Control::SelectStatusRecord(Json::Value &queryjson)
{
	return StatusRecordList::GetInstance()->SelectStatusRecord(queryjson);
}

Json::Value Control::SelectCommentListByAdmin(Json::Value &queryjson)
{
	// 如果不是管理员，无权查看
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;

	return CommentList::GetInstance()->SelectCommentListByAdmin(queryjson);
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
	if (!UserList::GetInstance()->IsOrdinaryUser(insertjson))
		return NoPermission;
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

// Json(ArticleId,CommentId)
Json::Value Control::DeleteComment(Json::Value &deletejson)
{
	// 如果不是用户，没有权限删除
	if (!UserList::GetInstance()->IsOrdinaryUser(deletejson))
		return NoPermission;

	string articleid = deletejson["ArticleId"].asString();

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
	string articletype = json["ArticleType"].asString();
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

Json::Value Control::SelectAnnouncementListByAdmin(Json::Value &queryjson)
{
	// 如果不是管理员，无权限
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;
	return AnnouncementList::GetInstance()->SelectAnnouncementListByAdmin(queryjson);
}

Json::Value Control::SelectAnnouncement(Json::Value &queryjson)
{
	return AnnouncementList::GetInstance()->SelectAnnouncement(queryjson);
}

Json::Value Control::SelectAnnouncementByEdit(Json::Value &queryjson)
{
	// 如果不是管理员，无权限
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;
	return AnnouncementList::GetInstance()->SelectAnnouncementByEdit(queryjson);
}

Json::Value Control::InsertAnnouncement(Json::Value &insertjson)
{
	// 如果不是管理员，无权限
	if (!UserList::GetInstance()->IsAdministrator(insertjson))
		return NoPermission;

	return AnnouncementList::GetInstance()->InsertAnnouncement(insertjson);
}

Json::Value Control::UpdateAnnouncement(Json::Value &updatejson)
{
	// 如果不是管理员，无权限
	if (!UserList::GetInstance()->IsAdministrator(updatejson))
		return NoPermission;

	return AnnouncementList::GetInstance()->UpdateAnnouncement(updatejson);
}
Json::Value Control::DeleteAnnouncement(Json::Value &deletejson)
{
	// 如果不是管理员，无权限
	if (!UserList::GetInstance()->IsAdministrator(deletejson))
		return NoPermission;

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
	// 如果不是管理员，无权限
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;

	return SolutionList::GetInstance()->SelectSolutionListByAdmin(queryjson);
}

Json::Value Control::SelectSolution(Json::Value &queryjson)
{
	return SolutionList::GetInstance()->SelectSolution(queryjson);
}

Json::Value Control::SelectSolutionByEdit(Json::Value &queryjson)
{
	// 如果不是用户，无权限
	if (!UserList::GetInstance()->IsOrdinaryUser(queryjson))
		return NoPermission;

	return SolutionList::GetInstance()->SelectSolutionByEdit(queryjson);
}

Json::Value Control::InsertSolution(Json::Value &insertjson)
{
	// 如果不是用户，无权限
	if (!UserList::GetInstance()->IsOrdinaryUser(insertjson))
		return NoPermission;

	return SolutionList::GetInstance()->InsertSolution(insertjson);
}

Json::Value Control::UpdateSolution(Json::Value &updatejson)
{
	// 如果不是作者或者管理员，无权限
	if (!UserList::GetInstance()->IsAuthor(updatejson))
		return NoPermission;

	return SolutionList::GetInstance()->UpdateSolution(updatejson);
}
Json::Value Control::DeleteSolution(Json::Value &deletejson)
{
	// 如果不是作者或者管理员，无权限
	if (!UserList::GetInstance()->IsAuthor(deletejson))
		return NoPermission;

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
	// 如果不是管理员，无权限
	if (!UserList::GetInstance()->IsAdministrator(queryjson))
		return NoPermission;

	return DiscussList::GetInstance()->SelectDiscussListByAdmin(queryjson);
}

Json::Value Control::SelectDiscuss(Json::Value &queryjson)
{
	return DiscussList::GetInstance()->SelectDiscuss(queryjson);
}

Json::Value Control::SelectDiscussByEdit(Json::Value &queryjson)
{
	// 如果不是用户，无权限
	if (!UserList::GetInstance()->IsOrdinaryUser(queryjson))
		return NoPermission;

	return DiscussList::GetInstance()->SelectDiscussByEdit(queryjson);
}

Json::Value Control::InsertDiscuss(Json::Value &insertjson)
{
	// 如果不是用户，无权限
	if (!UserList::GetInstance()->IsOrdinaryUser(insertjson))
		return NoPermission;

	return DiscussList::GetInstance()->InsertDiscuss(insertjson);
}

Json::Value Control::UpdateDiscuss(Json::Value &updatejson)
{
	// 如果不是作者或者管理员，无权限
	if (!UserList::GetInstance()->IsAuthor(updatejson))
		return NoPermission;

	return DiscussList::GetInstance()->UpdateDiscuss(updatejson);
}
Json::Value Control::DeleteDiscuss(Json::Value &deletejson)
{
	// 如果不是作者或者管理员，无权限
	if (!UserList::GetInstance()->IsAuthor(deletejson))
		return NoPermission;

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

	// 初始化用户权限
	UserList::GetInstance()->InitUserAuthority();

	// 初始化返回变量
	NoPermission["Result"] = "401";
	NoPermission["Reason"] = "无权限";
}

Control::~Control()
{
}