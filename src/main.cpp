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
    string querytype = "common";
    if (req.has_param("QueryType"))
    {
        querytype = req.get_param_value("QueryType");
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

    string matchstring = "";
    if (req.has_param("MatchString"))
    {
        pagesize = req.get_param_value("MatchString");
    }

    Json::Value queryjson;
    queryjson["QueryType"] = querytype;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;
    queryjson["MatchString"] = matchstring;

    Json::Value resvalue = control.SelectProblemSetInfo(queryjson);

    printf("doGetProblemSet end!!!\n");
    res.set_content(resvalue.toStyledString(), "json");
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
    string querytype = "all";

    if (req.has_param("QueryType"))
    {
        querytype = req.get_param_value("QueryType");
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

    string userid = "0";
    if (req.has_param("UserId"))
    {
        pagesize = req.get_param_value("UserId");
    }

    string problemid = "0";
    if (req.has_param("ProblemId"))
    {
        pagesize = req.get_param_value("ProblemId");
    }
    Json::Value queryjson;
    queryjson["QueryType"] = querytype;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;
    queryjson["UserId"] = userid;
    queryjson["ProblemId"] = problemid;
    Json::Value resvalue = control.SelectStatusRecordInfo(queryjson);
    printf("doGetProblemSet end!!!\n");
    res.set_content(resvalue.toStyledString(), "json");
}

void doGetDiscuss(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetDiscuss start!!!\n");
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
    queryjson["ParentId"] = parentid;
    queryjson["Page"] = page;
    queryjson["PageSize"] = pagesize;
    Json::Value resjson = control.SelectDiscuss(queryjson);
    printf("doGetDiscuss end!!!\n");
    res.set_content(resjson.toStyledString(), "json");
}

void doGetDiscussContent(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetDiscussContent start!!!\n");
    string discussid = "0";
    if (req.has_param("DiscussId"))
    {
        discussid = req.get_param_value("DiscussId");
    }
    Json::Value queryjson;
    queryjson["DiscussId"] = discussid;
    Json::Value resjson = control.SelectDiscussContent(queryjson);
    printf("doGetDiscussContent end!!!\n");
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
void doGetImage(const httplib::Request &req, httplib::Response &res)
{
    printf("doTest start!!!\n");
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
    printf("doTest end!!!\n");
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
    // 获取单个题目
    server.Get("/problem", doGetProblem);
    // 获取题库
    server.Get("/problemset", doGetProblemSet);
    // 获取状态记录
    server.Get("/statusrecord", doGetStatusRecord);
    // 提交代码
    server.Post("/problemcode", doPostCode);
    // 获取讨论
    server.Get("/discuss", doGetDiscuss);
    // 获取评论内容
    server.Get("/discuss/content", doGetDiscussContent);
    // 获取评论
    server.Get("/comment", doGetComment);
    // 获取图片资源
    server.Get(R"(/image/(\d+))", doGetImage);
    // 用户提交文章（讨论，题解）
    server.Post("/article/insert", doInsertArticle);

    server.Post("/comment/insert", doInsertComment);
    // 设置静态资源
    server.set_base_dir("../WWW");

    server.listen("0.0.0.0", 8081);
    return 0;
}