#include "HttpServer.h"
#include <iostream>
#include <fstream>
#include <typeinfo>
#include "Control.h"
#include "./log/log.h"

using namespace std;
Control control;

// 获取请求中的Token
string GetRequestToken(const httplib::Request &req)
{
    auto res = req.headers.find("Authorization");

    if (res != req.headers.end())
        return res->second;
    else
        return "0";
}

// 根据返回结果设置状态码
bool SetResponseStatus(const Json::Value &json, httplib::Response &res)
{
    string result = json["Result"].asString();
    if (result == "Success")
    {
        return true;
    }
    else if (result == "Fail")
    {
        return true;
    }
    else if (result == "400") // 请求参数有误
    {
        res.status = 400;
    }
    else if (result == "401") // 无权限
    {
        res.status = 401;
    }
    else if (result == "500") // 服务器出错啦
    {
        res.status = 500;
    }
    return true;
}

// 请求注册用户
void doRegisterUser(const httplib::Request &req, httplib::Response &res)
{
    printf("doRegister start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.RegisterUser(jsonvalue);

    printf("doGetProblem end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
// 请求登录用户
void doLoginUser(const httplib::Request &req, httplib::Response &res)
{
    printf("doLoginUser start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.LoginUser(jsonvalue);

    printf("doLoginUser end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

// 通过Token登录用户获取信息
void doGetUserInfoByToken(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserInfoByToken start!!!\n");
    Json::Value queryjson;

    queryjson["Token"] = GetRequestToken(req);
    Json::Value resjson = control.LoginUserByToken(queryjson);

    printf("doGetUserInfoByToken end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetUserRank(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserRank start!!!\n");
    Json::Value resjson;

    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetUserRank】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectUserRank(queryjson);
    }
    printf("doGetUserRank end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetUserInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserInfo start!!!\n");
    Json::Value resjson;

    if (!req.has_param("UserId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetUserInfo】缺少请求参数！");
    }
    else
    {
        string userid = req.get_param_value("UserId");
        Json::Value queryjson;
        queryjson["UserId"] = userid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectUserInfo(queryjson);
    }

    printf("doGetUserInfo end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
void doUpdateUserInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doUpdateUserInfo start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.UpdateUserInfo(jsonvalue);
    printf("doUpdateUserInfo end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetUserUpdateInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserUpdateInfo start!!!\n");

    Json::Value resjson;

    if (!req.has_param("UserId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetUserUpdateInfo】缺少请求参数！");
    }
    else
    {
        string userid = req.get_param_value("UserId");
        Json::Value queryjson;
        queryjson["UserId"] = userid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectUserUpdateInfo(queryjson);
    }
    printf("doGetUserUpdateInfo end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
void doGetUserSetInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserSetInfo start!!!\n");

    Json::Value resjson;

    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetUserSetInfo】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectUserSetInfo(queryjson);
    }
    printf("doGetUserSetInfo end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteUser(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteUser start!!!\n");

    Json::Value resjson;
    if (!req.has_param("UserId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doDeleteUser】缺少请求参数！");
    }
    else
    {
        string userid = req.get_param_value("UserId");

        Json::Value jsonvalue;
        jsonvalue["UserId"] = userid;
        jsonvalue["Token"] = GetRequestToken(req);
        resjson = control.DeleteUser(jsonvalue);
    }

    printf("doDeleteUser end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

// 返回网页请求的题目描述
void doGetProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblem start!!!\n");
    Json::Value resjson;
    if (!req.has_param("ProblemId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetProblem】缺少请求参数！");
    }
    else
    {
        string problemid = req.get_param_value("ProblemId");

        Json::Value queryjson;
        queryjson["ProblemId"] = problemid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectProblem(queryjson);
    }
    printf("doGetProblem end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

// 返回题库
void doGetProblemList(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemList start!!!\n");

    Json::Value resjson;
    if (!req.has_param("SearchInfo") || !req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "请求参数有误！";
        LOG_ERROR("【doGetProblemList】缺少请求参数！");
    }
    else
    {
        Json::Value serachinfo;
        Json::Reader reader;
        // 解析传入的json
        reader.parse(req.get_param_value("SearchInfo"), serachinfo);
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["SearchInfo"] = serachinfo;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectProblemList(queryjson);
    }
    printf("doGetProblemList end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
// 返回管理员的题库
void doGetProblemListByAdmin(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemListByAdmin start!!!\n");
    Json::Value resjson;
    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "请求参数有误！";
        LOG_ERROR("【doGetProblemListByAdmin】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");
        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectProblemListByAdmin(queryjson);
    }
    printf("doGetProblemListByAdmin end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

// 返回网页请求的题目描述
void doGetProblemInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemInfo start!!!\n");
    Json::Value resjson;

    if (!req.has_param("ProblemId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "请求参数有误！";
        LOG_ERROR("【doGetProblemInfo】缺少请求参数！");
    }
    else
    {
        string problemid = req.get_param_value("ProblemId");

        Json::Value queryjson;
        queryjson["ProblemId"] = problemid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectProblemInfoByAdmin(queryjson);
    }
    printf("doGetProblemInfo end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doEditProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doEditProblem start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["datainfo"]["Token"] = GetRequestToken(req);
    Json::Value resjson = control.EditProblem(jsonvalue["datainfo"]);
    printf("doEditProblem end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteProblem start!!!\n");
    Json::Value resjson;
    if (!req.has_param("ProblemId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "请求参数有误！";
        LOG_ERROR("【doDeleteProblem】缺少请求参数！");
    }
    else
    {
        string problemid = req.get_param_value("ProblemId");

        Json::Value deletejson;
        deletejson["ProblemId"] = problemid;
        deletejson["Token"] = GetRequestToken(req);
        resjson = control.DeleteProblem(deletejson);
    }
    printf("doDeleteProblem end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
// 前端提交代码进行判定并返回结果
void doPostCode(const httplib::Request &req, httplib::Response &res)
{
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.GetJudgeCode(jsonvalue);

    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetStatusRecordList(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetStatusRecordList start!!!\n");
    Json::Value resjson;

    if (!req.has_param("SearchInfo") || !req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetStatusRecordList】缺少请求参数！");
    }
    else
    {
        Json::Value searchinfo;
        Json::Reader reader;
        // 解析传入的json
        reader.parse(req.get_param_value("SearchInfo"), searchinfo);

        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["SearchInfo"] = searchinfo;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectStatusRecordList(queryjson);
    }
    printf("doGetStatusRecordList end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
void doGetStatusRecord(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetStatusRecordInfo start!!!\n");

    Json::Value resjson;
    if (!req.has_param("SubmitId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetStatusRecord】缺少请求参数！");
    }
    else
    {
        string submitid = req.get_param_value("SubmitId");

        Json::Value queryjson;
        queryjson["SubmitId"] = submitid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectStatusRecord(queryjson);
    }
    printf("doGetStatusRecordInfo end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
// ------------------公告------------------------------
void doGetAnnouncementList(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetAnnouncementList start!!!\n");

    Json::Value resjson;
    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetAnnouncementList】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectAnnouncementList(queryjson);
    }

    printf("doGetAnnouncementList end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetAnnouncementListByAdmin(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetAnnouncementListByAdmin start!!!\n");

    Json::Value resjson;
    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetAnnouncementListByAdmin】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectAnnouncementListByAdmin(queryjson);
    }

    printf("doGetAnnouncementListByAdmin end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetAnnouncement start!!!\n");
    Json::Value resjson;

    if (!req.has_param("AnnouncementId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetAnnouncement】缺少请求参数！");
    }
    else
    {
        string announcementid = req.get_param_value("AnnouncementId");
        Json::Value queryjson;
        queryjson["AnnouncementId"] = announcementid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectAnnouncement(queryjson);
    }
    printf("doGetAnnouncement end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doSelectAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doSelectAnnouncement start!!!\n");

    Json::Value resjson;
    if (!req.has_param("AnnouncementId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doSelectAnnouncement】缺少请求参数！");
    }
    else
    {
        string announcementid = req.get_param_value("AnnouncementId");

        Json::Value queryjson;
        queryjson["AnnouncementId"] = announcementid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectAnnouncementByEdit(queryjson);
    }
    printf("doSelectAnnouncement end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doInsertAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doInsertAnnouncement start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.InsertAnnouncement(jsonvalue);
    printf("doInsertAnnouncement end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doUpdateAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doUpdateAnnouncement start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.UpdateAnnouncement(jsonvalue);
    printf("doUpdateAnnouncement end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteAnnouncement start!!!\n");
    Json::Value resjson;

    if (!req.has_param("AnnouncementId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doDeleteAnnouncement】缺少请求参数！");
    }
    else
    {
        string announcementid = req.get_param_value("AnnouncementId");
        Json::Value deletejson;
        deletejson["AnnouncementId"] = announcementid;
        deletejson["Token"] = GetRequestToken(req);
        resjson = control.DeleteAnnouncement(deletejson);
    }
    printf("doDeleteAnnouncement end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

// ------------------题解------------------------------
void doGetSolutionList(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetSolutionList start!!!\n");
    Json::Value resjson;

    if (!req.has_param("SearchInfo") || !req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetSolutionList】缺少请求参数！");
    }
    else
    {
        Json::Value searchinfo;
        Json::Reader reader;
        // 解析传入的json
        reader.parse(req.get_param_value("SearchInfo"), searchinfo);
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");
        Json::Value queryjson;
        queryjson["SearchInfo"] = searchinfo;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectSolutionList(queryjson);
    }
    printf("doGetSolutionList end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetSolutionListByAdmin(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetSolutionListByAdmin start!!!\n");
    Json::Value resjson;
    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetSolutionListByAdmin】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectSolutionListByAdmin(queryjson);
    }
    printf("doGetSolutionListByAdmin end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
void doGetSolution(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetSolution start!!!\n");
    Json::Value resjson;

    if (!req.has_param("SolutionId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetSolution】缺少请求参数！");
    }
    else
    {
        string solutionid = req.get_param_value("SolutionId");
        Json::Value queryjson;
        queryjson["SolutionId"] = solutionid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectSolution(queryjson);
    }
    printf("doGetSolution end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doSelectSolutionByEdit(const httplib::Request &req, httplib::Response &res)
{
    printf("doSelectSolutionByEdit start!!!\n");
    Json::Value resjson;

    if (!req.has_param("SolutionId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doSelectSolutionByEdit】缺少请求参数！");
    }
    else
    {
        string solutionid = req.get_param_value("SolutionId");
        Json::Value queryjson;
        queryjson["SolutionId"] = solutionid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectSolutionByEdit(queryjson);
    }
    printf("doSelectSolutionByEdit end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doInsertSolution(const httplib::Request &req, httplib::Response &res)
{
    printf("doInsertSolution start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.InsertSolution(jsonvalue);
    printf("doInsertSolution end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doUpdateSolution(const httplib::Request &req, httplib::Response &res)
{
    printf("doUpdateSolution start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.UpdateSolution(jsonvalue);
    printf("doUpdateSolution end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteSolution(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteSolution start!!!\n");
    Json::Value resjson;

    if (!req.has_param("SolutionId") || !req.has_param("UserId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doDeleteSolution】缺少请求参数！");
    }
    else
    {
        string solutionid = req.get_param_value("SolutionId");
        string userid = req.get_param_value("UserId");
        Json::Value deletejson;
        deletejson["SolutionId"] = solutionid;
        deletejson["UserId"] = userid;
        deletejson["Token"] = GetRequestToken(req);
        resjson = control.DeleteSolution(deletejson);
    }
    printf("doDeleteSolution end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

// ------------------讨论------------------------------
void doGetDiscussList(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetDiscussList start!!!\n");
    Json::Value resjson;
    if (!req.has_param("SearchInfo") || !req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetDiscussList】缺少请求参数！");
    }
    else
    {
        Json::Value searchinfo;
        Json::Reader reader;
        // 解析传入的json
        reader.parse(req.get_param_value("SearchInfo"), searchinfo);
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");
        Json::Value queryjson;
        queryjson["SearchInfo"] = searchinfo;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectDiscussList(queryjson);
    }
    printf("doGetDiscussList end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetDiscussListByAdmin(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetDiscussListByAdmin start!!!\n");
    Json::Value resjson;
    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetDiscussListByAdmin】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");
        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectDiscussListByAdmin(queryjson);
    }
    printf("doGetDiscussListByAdmin end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
void doGetDiscuss(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetDiscuss start!!!\n");
    Json::Value resjson;
    if (!req.has_param("DiscussId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetDiscuss】缺少请求参数！");
    }
    else
    {
        string discussid = req.get_param_value("DiscussId");
        Json::Value queryjson;
        queryjson["DiscussId"] = discussid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectDiscuss(queryjson);
    }
    printf("doGetDiscuss end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doSelectDiscussByEdit(const httplib::Request &req, httplib::Response &res)
{
    printf("doSelectDiscussByEdit start!!!\n");
    Json::Value resjson;
    if (!req.has_param("DiscussId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doSelectDiscussByEdit】缺少请求参数！");
    }
    else
    {
        string discussid = req.get_param_value("DiscussId");
        Json::Value queryjson;
        queryjson["DiscussId"] = discussid;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectDiscussByEdit(queryjson);
    }
    printf("doSelectDiscussByEdit end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doInsertDiscuss(const httplib::Request &req, httplib::Response &res)
{
    printf("doInsertSolution start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.InsertDiscuss(jsonvalue);
    printf("doInsertSolution end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doUpdateDiscuss(const httplib::Request &req, httplib::Response &res)
{
    printf("doUpdateSolution start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Token"] = GetRequestToken(req);
    Json::Value resjson = control.UpdateDiscuss(jsonvalue);
    printf("doUpdateSolution end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteDiscuss(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteDiscuss start!!!\n");
    Json::Value resjson;

    if (!req.has_param("DiscussId") || !req.has_param("UserId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doDeleteDiscuss】缺少请求参数！");
    }
    else
    {
        string discussid = req.get_param_value("DiscussId");
        string userid = req.get_param_value("UserId");

        Json::Value deletejson;
        deletejson["DiscussId"] = discussid;
        deletejson["UserId"] = userid;
        deletejson["Token"] = GetRequestToken(req);
        resjson = control.DeleteDiscuss(deletejson);
    }
    printf("doDeleteDiscuss end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doSelectCommentListByAdmin(const httplib::Request &req, httplib::Response &res)
{
    printf("doSelectCommentListByAdmin start!!!\n");
    Json::Value resjson;

    if (!req.has_param("Page") || !req.has_param("PageSize"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doSelectCommentListByAdmin】缺少请求参数！");
    }
    else
    {
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");

        Json::Value queryjson;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.SelectCommentListByAdmin(queryjson);
    }
    printf("doSelectCommentListByAdmin end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetComment(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetComment start!!!\n");
    Json::Value resjson;

    if (!req.has_param("Type") || !req.has_param("ParentId") || !req.has_param("Page") || !req.has_param("PageSize") || !req.has_param("SonNum"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doGetComment】缺少请求参数！");
    }
    else
    {
        string type = req.get_param_value("Type");
        string parentid = req.get_param_value("ParentId");
        string page = req.get_param_value("Page");
        string pagesize = req.get_param_value("PageSize");
        string sonsnum = req.get_param_value("SonNum");
        Json::Value queryjson;
        queryjson["Type"] = type;
        queryjson["ParentId"] = parentid;
        queryjson["Page"] = page;
        queryjson["PageSize"] = pagesize;
        queryjson["SonNum"] = sonsnum;
        queryjson["Token"] = GetRequestToken(req);
        resjson = control.GetComment(queryjson);
    }

    printf("doGetComment end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doInsertComment(const httplib::Request &req, httplib::Response &res)
{
    printf("doInsertComment start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    jsonvalue["Info"]["Token"] = GetRequestToken(req);
    Json::Value resjson = control.InsertComment(jsonvalue["Info"]);
    printf("doInsertComment end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}
void doDeleteComment(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteComment start!!!\n");
    Json::Value resjson;
    if (!req.has_param("ArticleId") || !req.has_param("CommentId"))
    {
        resjson["Result"] = "400";
        resjson["Reason"] = "缺少请求参数！";
        LOG_ERROR("【doDeleteComment】缺少请求参数！");
    }
    else
    {
        string articleid = req.get_param_value("ArticleId");
        string commentid = req.get_param_value("CommentId");
        Json::Value deletejson;
        deletejson["ArticleId"] = articleid;
        deletejson["CommentId"] = commentid;
        deletejson["Token"] = GetRequestToken(req);
        resjson = control.DeleteComment(deletejson);
    }
    printf("doDeleteComment end!!!\n");
    SetResponseStatus(resjson, res);
    res.set_content(resjson.toStyledString(), "json");
}

void doGetImage(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetImage start!!!\n");
    int index = stoi(req.matches[1]);
    string path = "../WWW/image/avatar" + to_string(index) + ".webp";
    ifstream infile;
    infile.open(path.data());
    if (!infile.is_open())
    {
        string str = "图片获取失败";
        res.set_content(str, "text");
    }
    string image((istreambuf_iterator<char>(infile)),
                 (istreambuf_iterator<char>()));
    printf("doGetImage end!!!\n");
    res.set_content(image, "webp");
}

void doGetTags(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetTags start!!!\n");
    Json::Value queryjson;
    string tagtype = req.get_param_value("TagType");
    queryjson["TagType"] = tagtype;

    Json::Value resjson = control.GetTags(queryjson);
    printf("doGetTags end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void HttpServer::Run()
{
    // 创建日志文件系统
    Log::Instance()->init(1, "./log", ".log", 1024);
    LOG_INFO("========== Server init ==========");

    using namespace httplib;
    Server server;
    // -----------------用户----------------
    // 注册用户
    server.Post("/user/register", doRegisterUser);
    // 登录用户
    server.Post("/user/login", doLoginUser);
    // 返回用户信息 Token登录
    server.Get("/user/tokenlogin", doGetUserInfoByToken);
    // 返回用户排名
    server.Get("/user/rank", doGetUserRank);
    // 返回用户信息，用于主页展示
    server.Get("/user/home", doGetUserInfo);
    // 更新用户信息
    server.Post("/user/update", doUpdateUserInfo);
    // 返回用户信息，用于编辑修改
    server.Get("/user/updateinfo", doGetUserUpdateInfo);
    // 分页获取用户信息
    server.Get("/userlist/admin", doGetUserSetInfo);
    // 更新用户信息
    server.Delete("/user", doDeleteUser);

    // ----------------题目------------------
    // 获取单个题目
    server.Get("/problem", doGetProblem);
    // 获取题库
    server.Get("/problemlist", doGetProblemList);
    // 管理员获取题库
    server.Get("/problemlist/admin", doGetProblemListByAdmin);
    // 获取单个题目详细信息
    server.Get("/problem/select", doGetProblemInfo);
    // 编辑题目 包含添加题目，修改题目(接口修改一下)
    server.Post("/problem/edit", doEditProblem);
    // 删除题目
    server.Delete("/problem", doDeleteProblem);

    // ---------------测评-----------------
    // 获取状态记录
    server.Get("/statusrecordlist", doGetStatusRecordList);
    // 获取一条测评记录
    server.Get("/statusrecord", doGetStatusRecord);

    // 提交代码
    server.Post("/problemcode", doPostCode);

    // --------------公告--------------------
    // 获取公告列表
    server.Get("/announcementlist", doGetAnnouncementList);

    // 获取公告列表
    server.Get("/announcementlist/admin", doGetAnnouncementListByAdmin);

    // 获取公告内容
    server.Get("/announcement", doGetAnnouncement);

    // 获取公告信息用于编辑
    server.Get("/announcement/select", doSelectAnnouncement);

    // 用户提交公告
    server.Post("/announcement/insert", doInsertAnnouncement);

    // 用户修改公告
    server.Post("/announcement/update", doUpdateAnnouncement);

    // 用户删除公告
    server.Delete("/announcement", doDeleteAnnouncement);

    // -------------题解------------------------
    // 获取题解列表
    server.Get("/solutionlist", doGetSolutionList);

    // 获取题解列表
    server.Get("/solutionlist/admin", doGetSolutionListByAdmin);

    // 获取题解内容
    server.Get("/solution", doGetSolution);

    // 获取题解信息用于编辑
    server.Get("/solution/select", doSelectSolutionByEdit);

    // 用户题解公告
    server.Post("/solution/insert", doInsertSolution);

    // 用户修改题解
    server.Post("/solution/update", doUpdateSolution);

    // 用户删除题解
    server.Delete("/solution", doDeleteSolution);

    // -------------讨论------------------------
    // 获取讨论列表
    server.Get("/discusslist", doGetDiscussList);

    // 管理员获取讨论列表
    server.Get("/discusslist/admin", doGetDiscussListByAdmin);

    // 获取讨论内容
    server.Get("/discuss", doGetDiscuss);

    // 获取讨论信息用于编辑
    server.Get("/discuss/select", doSelectDiscussByEdit);

    // 用户提交讨论
    server.Post("/discuss/insert", doInsertDiscuss);

    // 用户修改讨论
    server.Post("/discuss/update", doUpdateDiscuss);

    // 用户删除讨论
    server.Delete("/discuss", doDeleteDiscuss);

    // ---------------评论--------------------
    // 管理员获取评论
    server.Get("/commentlist/admin", doSelectCommentListByAdmin);
    // 获取评论
    server.Get("/comment", doGetComment);
    // 提交评论
    server.Post("/comment/insert", doInsertComment);
    // 删除评论
    server.Delete("/comment", doDeleteComment);

    // 获取图片资源
    server.Get(R"(/image/(\d+))", doGetImage);
    // 获取标签
    server.Get("/tags", doGetTags);
    // 设置静态资源
    server.set_base_dir("../WWW");

    server.listen("0.0.0.0", 8081);
}
HttpServer::HttpServer()
{
}
HttpServer::~HttpServer()
{
}