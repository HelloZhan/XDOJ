#include "httplib.h"
#include <iostream>
#include <string>
#include <fstream>
#include "ProblemSet.hpp"
#include <typeinfo>
#include <map>
#include "Judger.h"

using namespace std;
ProblemSet problemset;
Judger judger;

void doGetHi(const httplib::Request &req, httplib::Response &res)
{
    string code = "#include <iostream>";
    res.set_content(code, "text");
}
// 返回网页请求的问题描述
void doGetProblem(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblem start!!!\n");
    string id = "1";
    if (req.has_param("id"))
    {
        id = req.get_param_value("id");
    }
    string buf = problemset.getproblemdescription(id);
    printf("doGetProblem end!!!\n");
    res.set_content(buf, "text");
}
// 返回题库
void doGetProblemSet(const httplib::Request &req, httplib::Response &res)
{
    printf("doGetProblemSet start!!!\n");
    Json::Value resvalue;
    resvalue = problemset.getproblemset();
    printf("doGetProblemSet end!!!\n");
    res.set_content(resvalue.toStyledString(), "json");
}
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
    runjson["judgenum"] = problemset.getproblemtestnum(jsonvalue["id"].asString());

    Json::Value resjson = judger.Run(runjson);

    string result = resjson["result"].asString();
    res.set_content(result, "text");
}
int main()
{
    using namespace httplib;
    problemset.init();
    Server server;
    server.Get("/code", doGetHi);
    server.Get("/problem", doGetProblem);
    server.Get("/problemset", doGetProblemSet);
    server.Post("/problecode", doPostCode);
    server.listen("0.0.0.0", 8081);

    return 0;
}