#include "../include/httplib.h"
#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>
#include <map>
#include "Control.h"

using namespace std;
Control control;

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
int main()
{
    using namespace httplib;
    Server server;
    // 获取单个题目
    server.Get("/problem", doGetProblem);
    // 获取题库
    server.Get("/problemset", doGetProblemSet);
    // 获取状态记录
    server.Get("/statusrecord", doGetStatusRecord);
    // 提交代码
    server.Post("/problecode", doPostCode);

    server.listen("0.0.0.0", 8081);

    return 0;
}