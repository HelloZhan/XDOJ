#ifndef PROBLEMSET_H
#define PROBLEMSET_H

#include <map>
#include <string>
#include <jsoncpp/json/json.h>
class Problem;

class ProblemSet
{
public:
    // 局部静态特性的方式实现单实例
    static ProblemSet &GetInstance();

    void Init();
    std::string getProblemDescription(std::string id);

    // 获取题库数据
    Json::Value getProblemSet();

    // 通过普通查询获取题库数据
    Json::Value getProblemSetInfoByCommon(std::string offest, std::string limit);

    // 获取题目的测试数量
    int getProblemJudgeNum(std::string id);

private:
    ProblemSet();
    ~ProblemSet();

private:
    std::map<std::string, Problem *> heap;
    int ProblemNum;
};

#endif