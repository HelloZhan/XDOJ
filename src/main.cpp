#include "../include/httplib.h"
#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>
#include <map>
#include "Control.h"
#include <vector>

using namespace std;
Control control;

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
    res.set_content(resjson.toStyledString(), "json");
}
void doGetUserRank(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserRank start!!!\n");
    string page = "1";
    if (req.has_param("Page"))
    {
        page = req.get_param_value("Page");
    }

    string pagesize = "10";
    if (req.has_param("PageSize"))
    {
        pagesize = req.get_param_value("PageSize");
    }

    Json::Value queryjson;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;
    Json::Value resjson = control.SelectUserRank(queryjson);
    printf("doGetUserRank end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetUserInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserInfo start!!!\n");
    string userid = "1";
    if (req.has_param("UserId"))
    {
        userid = req.get_param_value("UserId");
    }

    Json::Value queryjson;
    queryjson["UserId"] = userid;
    Json::Value resjson = control.SelectUserInfo(queryjson);
    printf("doGetUserInfo end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}
void doUpdateUserInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doUpdateUserInfo start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.UpdateUserInfo(jsonvalue);
    printf("doUpdateUserInfo end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetUserUpdateInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserUpdateInfo start!!!\n");
    string userid = "1";
    if (req.has_param("UserId"))
    {
        userid = req.get_param_value("UserId");
    }

    Json::Value queryjson;
    queryjson["UserId"] = userid;
    Json::Value resjson = control.SelectUserUpdateInfo(queryjson);
    printf("doGetUserUpdateInfo end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}
void doGetUserSetInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetUserSetInfo start!!!\n");

    string page = "1";
    if (req.has_param("Page"))
    {
        page = req.get_param_value("Page");
    }

    string pagesize = "25";
    if (req.has_param("PageSize"))
    {
        pagesize = req.get_param_value("PageSize");
    }
    Json::Value queryjson;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;

    Json::Value resjson = control.SelectUserSetInfo(queryjson);
    printf("doGetUserSetInfo end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteUser(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteUser start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.DeleteUser(jsonvalue);
    printf("doDeleteUser end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

// 返回网页请求的题目描述
void doGetProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblem start!!!\n");
    // 默认为 1
    string ProblemId = "1";
    if (req.has_param("ProblemId"))
    {
        ProblemId = req.get_param_value("ProblemId");
    }
    cout << "请求的ID：" << ProblemId << endl;
    string buf = control.GetProblemDescription(ProblemId);
    printf("doGetProblem end!!!\n");
    res.set_content(buf, "text");
}

// 返回题库
void doGetProblemSet(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemSet start!!!\n");
    Json::Value serachinfo;
    if (req.has_param("SearchInfo"))
    {
        string info = req.get_param_value("SearchInfo");
        Json::Reader reader;
        // 解析传入的json
        reader.parse(info, serachinfo);
    }
    string page = "1";
    if (req.has_param("Page"))
    {
        page = req.get_param_value("Page");
    }

    string pagesize = "25";
    if (req.has_param("PageSize"))
    {
        pagesize = req.get_param_value("PageSize");
    }
    Json::Value queryjson;
    queryjson["SearchInfo"] = serachinfo;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;

    Json::Value resjson = control.SelectProblemSetInfo(queryjson);
    printf("doGetProblemSet end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

// 返回网页请求的题目描述
void doGetProblemInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemInfo start!!!\n");
    // 默认为 1
    string ProblemId = "1";
    if (req.has_param("ProblemId"))
    {
        ProblemId = req.get_param_value("ProblemId");
    }
    Json::Value queryjson;
    queryjson["ProblemId"] = ProblemId;
    Json::Value resjson = control.SelectProblemInfoByAdmin(queryjson);
    printf("doGetProblemInfo end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doEditProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doEditProblem start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.EditProblem(jsonvalue["datainfo"]);
    printf("doEditProblem end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteProblem start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.DeleteProblem(jsonvalue);
    printf("doDeleteProblem end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}
// 前端提交代码进行判定并返回结果
void doPostCode(const httplib::Request &req, httplib::Response &res)
{
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);

    Json::Value resjson = control.GetJudgeCode(jsonvalue);

    res.set_content(resjson.toStyledString(), "json");
}

void doGetStatusRecord(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetStatusRecord start!!!\n");
    Json::Value searchinfo;

    if (req.has_param("SearchInfo"))
    {
        Json::Reader reader;
        // 解析传入的json
        reader.parse(req.get_param_value("SearchInfo"), searchinfo);
    }

    string page = "1";
    if (req.has_param("Page"))
    {
        page = req.get_param_value("Page");
    }

    string pagesize = "25";
    if (req.has_param("PageSize"))
    {
        pagesize = req.get_param_value("PageSize");
    }

    Json::Value queryjson;
    queryjson["SearchInfo"] = searchinfo;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;
    Json::Value resvalue = control.SelectStatusRecordInfo(queryjson);
    printf("doGetProblemSet end!!!\n");
    res.set_content(resvalue.toStyledString(), "json");
}
void doGetStatusRecordInfo(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetStatusRecordInfo start!!!\n");
    // 默认为 1
    string submitid = "1";
    if (req.has_param("SubmitId"))
    {
        submitid = req.get_param_value("SubmitId");
    }
    Json::Value queryjson;
    queryjson["SubmitId"] = submitid;
    Json::Value resjson = control.SelectOneStatusRecord(queryjson);
    printf("doGetStatusRecordInfo end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetAnnouncement start!!!\n");
    string page = "1";
    if (req.has_param("Page"))
    {
        page = req.get_param_value("Page");
    }

    string pagesize = "10";
    if (req.has_param("PageSize"))
    {
        pagesize = req.get_param_value("PageSize");
    }
    Json::Value queryjson;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;
    Json::Value resjson = control.SelectAnnouncement(queryjson);

    printf("doGetAnnouncement end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetAnnouncementContent(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetAnnouncementContent start!!!\n");

    string announcementid = "0";
    if (req.has_param("AnnouncementId"))
    {
        announcementid = req.get_param_value("AnnouncementId");
    }

    Json::Value queryjson;
    queryjson["AnnouncementId"] = announcementid;

    Json::Value resjson = control.SelectAnnouncementContent(queryjson);
    printf("doGetAnnouncementContent end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doSelectAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doSelectAnnouncement start!!!\n");

    string announcementid = "0";
    if (req.has_param("AnnouncementId"))
    {
        announcementid = req.get_param_value("AnnouncementId");
    }
    Json::Value queryjson;
    queryjson["AnnouncementId"] = announcementid;

    Json::Value resjson = control.SelectAnnouncementByEdit(queryjson);
    printf("doSelectAnnouncement end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doInsertAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doInsertAnnouncement start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.InsertAnnouncement(jsonvalue);
    printf("doInsertAnnouncement end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doUpdateAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doUpdateAnnouncement start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.UpdateAnnouncement(jsonvalue);
    printf("doUpdateAnnouncement end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteAnnouncement(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteAnnouncement start!!!\n");

    string announcementid = "0";
    if (req.has_param("AnnouncementId"))
    {
        announcementid = req.get_param_value("AnnouncementId");
    }
    Json::Value deletejson;
    deletejson["AnnouncementId"] = announcementid;

    Json::Value resjson = control.DeleteAnnouncement(deletejson);
    printf("doDeleteAnnouncement end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetArticle(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetArticle start!!!\n");
    string articletype = "Discuss";
    if (req.has_param("ArticleType"))
    {
        articletype = req.get_param_value("ArticleType");
    }

    string page = "1";
    if (req.has_param("Page"))
    {
        page = req.get_param_value("Page");
    }

    string pagesize = "10";
    if (req.has_param("PageSize"))
    {
        pagesize = req.get_param_value("PageSize");
    }

    string parentid = "0";
    if (req.has_param("ParentId"))
    {
        parentid = req.get_param_value("ParentId");
    }
    Json::Value queryjson;
    queryjson["ArticleType"] = articletype;
    queryjson["ParentId"] = parentid;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;
    Json::Value resjson = control.SelectArticle(queryjson);
    printf("doGetArticle end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetArticleContent(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetArticleContent start!!!\n");

    string articletype = "Discuss";
    if (req.has_param("ArticleType"))
    {
        articletype = req.get_param_value("ArticleType");
    }
    string articleid = "0";
    if (req.has_param("ArticleId"))
    {
        articleid = req.get_param_value("ArticleId");
    }

    Json::Value queryjson;
    queryjson["ArticleType"] = articletype;
    queryjson["ArticleId"] = articleid;

    Json::Value resjson = control.SelectArticleContent(queryjson);
    printf("doGetArticleContent end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doSelectArticle(const httplib::Request &req, httplib::Response &res)
{
    printf("doSelectArticle start!!!\n");

    string articletype = "Discuss";
    if (req.has_param("ArticleType"))
    {
        articletype = req.get_param_value("ArticleType");
    }
    string articleid = "0";
    if (req.has_param("ArticleId"))
    {
        articleid = req.get_param_value("ArticleId");
    }
    Json::Value queryjson;
    queryjson["ArticleType"] = articletype;
    queryjson["ArticleId"] = articleid;

    Json::Value resjson = control.SelectArticleByEdit(queryjson);
    printf("doSelectArticle end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}
void doGetImage(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetImage start!!!\n");
    int index = stoi(req.matches[1]);
    string path = "../../WWW/image/avatar_" + to_string(index) + ".webp";
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
void doInsertArticle(const httplib::Request &req, httplib::Response &res)
{
    printf("doInsertArticle start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.InsertArticle(jsonvalue);
    printf("doInsertArticle end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doUpdateArticle(const httplib::Request &req, httplib::Response &res)
{
    printf("doUpdateArticle start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.UpdateArticle(jsonvalue);
    printf("doUpdateArticle end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doDeleteArticle(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteArticle start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.DeleteArticle(jsonvalue);
    printf("doDeleteArticle end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetComment(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetComment start!!!\n");
    Json::Value queryjson;
    string type = req.get_param_value("Type");
    string parentid = req.get_param_value("ParentId");
    string skip = req.get_param_value("Skip");
    string limit = req.get_param_value("Limit");
    string sonsnum = req.get_param_value("SonNum");
    queryjson["Type"] = type;
    queryjson["ParentId"] = parentid;
    queryjson["Skip"] = skip;
    queryjson["Limit"] = limit;
    queryjson["SonNum"] = sonsnum;

    Json::Value resjson = control.GetComment(queryjson);
    printf("doGetComment end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doInsertComment(const httplib::Request &req, httplib::Response &res)
{
    printf("doInsertComment start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.InsertComment(jsonvalue["Info"]);
    printf("doInsertComment end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}
void doDeleteComment(const httplib::Request &req, httplib::Response &res)
{
    printf("doDeleteComment start!!!\n");
    Json::Value jsonvalue;
    Json::Reader reader;
    // 解析传入的json
    reader.parse(req.body, jsonvalue);
    Json::Value resjson = control.DeleteComment(jsonvalue);
    printf("doDeleteComment end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
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

int main()
{
    using namespace httplib;
    Server server;
    // 注册用户
    server.Post("/register", doRegisterUser);
    // 登录用户
    server.Post("/login", doLoginUser);
    // 返回用户排名
    server.Get("/userrank", doGetUserRank);
    // 返回用户信息，用于主页展示
    server.Get("/user/userhome", doGetUserInfo);
    // 更新用户信息
    server.Post("/user/usersetting", doUpdateUserInfo);
    // 返回用户信息，用于编辑修改
    server.Get("/user/updateinfo", doGetUserUpdateInfo);
    // 分页获取用户信息
    server.Get("/userset", doGetUserSetInfo);
    // 更新用户信息
    server.Post("/user/delete", doDeleteUser);

    // 获取单个题目
    server.Get("/problem", doGetProblem);
    // 获取题库
    server.Get("/problemset", doGetProblemSet);

    // 获取单个题目详细信息
    server.Get("/problem/select", doGetProblemInfo);
    // 编辑题目 包含添加题目，修改题目
    server.Post("/problem/edit", doEditProblem);
    // 删除题目
    server.Post("/problem/delete", doDeleteProblem);

    // 获取状态记录
    server.Get("/statusrecord", doGetStatusRecord);
    // 获取一条测评记录
    server.Get("/statusrecord/info", doGetStatusRecordInfo);

    // 提交代码
    server.Post("/problemcode", doPostCode);
    // 获取图片资源
    server.Get(R"(/image/(\d+))", doGetImage);

    // --------------公告--------------------
    // 获取公告列表
    server.Get("/announcement", doGetAnnouncement);

    // 获取公告内容
    server.Get("/announcement/content", doGetAnnouncementContent);

    // 获取公告信息用于编辑
    server.Get("/announcement/select", doSelectAnnouncement);

    // 用户提交公告
    server.Post("/announcement/insert", doInsertAnnouncement);

    // 用户修改文章（讨论，题解）
    server.Post("/announcement/update", doUpdateAnnouncement);

    // 用户删除公告
    server.Delete("/announcement", doDeleteAnnouncement);
    // -------------文章--------------------------

    // 获取文章
    server.Get("/article", doGetArticle);
    // 获取评论内容
    server.Get("/article/content", doGetArticleContent);
    // 获取文章信息
    server.Get("/article/select", doSelectArticle);
    // 用户提交文章（讨论，题解）
    server.Post("/article/insert", doInsertArticle);
    // 用户修改文章（讨论，题解）
    server.Post("/article/update", doUpdateArticle);
    // 用户删除文章（讨论，题解）
    server.Post("/article/delete", doDeleteArticle);

    // ---------------评论--------------------
    // 获取评论
    server.Get("/comment", doGetComment);
    // 提交评论
    server.Post("/comment/insert", doInsertComment);
    // 删除评论
    server.Post("/comment/delete", doDeleteComment);

    // 获取标签
    server.Get("/tags", doGetTags);
    // 设置静态资源
    server.set_base_dir("../WWW");

    server.listen("0.0.0.0", 8081);
    return 0;
}