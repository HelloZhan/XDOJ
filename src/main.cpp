#include "httplib.h"
#include <iostream>
#include <string>
#include <fstream>
#include "ProblemSet.h"
#include <typeinfo>
#include <map>
#include "Judger.h"

using namespace std;
Judger judger;

// 返回网页请求的题目描述
void doGetProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblem start!!!\n");
    string id = "1";
    if (req.has_param("id"))
    {
        id = req.get_param_value("id");
    }
    string buf = ProblemSet::GetInstance().getProblemDescription(id);
    printf("doGetProblem end!!!\n");
    res.set_content(buf, "text");
}

// 返回题库
void doGetProblemSet(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemSet start!!!\n");
    Json::Value resvalue;
    resvalue = ProblemSet::GetInstance().getProblemSet();
    printf("doGetProblemSet end!!!\n");
    res.set_content(resvalue.toStyledString(), "json");
}
// 前端提交代码进行判定并返回结果
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
    runjson["judgenum"] = ProblemSet::GetInstance().getProblemJudgeNum(jsonvalue["id"].asString());
    runjson["timelimit"] = 2000;
    runjson["memorylimit"] = 134217728;

    Json::Value resjson = judger.Run(runjson);
    res.set_content(resjson.toStyledString(), "json");
}
int main()
{
    using namespace httplib;
    ProblemSet::GetInstance().Init();
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