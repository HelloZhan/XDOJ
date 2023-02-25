#include "httplib.h"
#include <iostream>
#include <string>
#include <fstream>
#include "ProblemSet.h"
#include <typeinfo>
#include <map>
#include "Judger.h"

using namespace std;
ProblemSet problemset;
Judger judger;

// 返回网页请求的问题描述
void doGetProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblem start!!!\n");
    string id = "1";
    if (req.has_param("id"))
    {
        id = req.get_param_value("id");
    }
    string buf = problemset.getProblemDescription(id);
    printf("doGetProblem end!!!\n");
    res.set_content(buf, "text");
}
// 返回题库
void doGetProblemSet(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemSet start!!!\n");
    Json::Value resvalue;
    resvalue = problemset.getProblemSet();
    printf("doGetProblemSet end!!!\n");
    res.set_content(resvalue.toStyledString(), "json");
}
// 提交代码
void doPostCode(const httplib::Request &req, httplib::Response &res)
{
    printf("req.body = %s\n", req.body.data());
    Json::Value jsonvalue;

    Json::Reader reader;
    reader.parse(req.body, jsonvalue);

    Json::Value runjson;
    runjson["code"] = jsonvalue["code"];
    runjson["runid"] = "100";
    runjson["problemid"] = jsonvalue["id"];
    runjson["language"] = "c_cpp";
    runjson["judgenum"] = problemset.getProblemJudgeNum(jsonvalue["id"].asString());

    Json::Value resjson = judger.Run(runjson);

    string result = resjson["result"].asString();
    res.set_content(result, "text");
}
int main()
{
    using namespace httplib;
    problemset.Init();
    Server server;
    // 获取单个题目
    server.Get("/problem", doGetProblem);
    // 获取题库
    server.Get("/problemset", doGetProblemSet);
    // 提交代码
    server.Post("/problecode", doPostCode);
    server.listen("0.0.0.0", 8081);

    return 0;
}