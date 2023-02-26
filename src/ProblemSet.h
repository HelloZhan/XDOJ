#ifndef PROBLEMSET_H
#define PROBLEMSET_H

#include <map>
#include <string>
#include <jsoncpp/json/json.h>
class Problem;

class ProblemSet
{
public:
    ProblemSet();
    void Init();
    std::string getProblemDescription(std::string id);

    // 获取题库数据
    Json::Value getProblemSet();

    // 获取题目的测试数量
    int getProblemJudgeNum(std::string id);

    ~ProblemSet();

private:
    std::map<std::string, Problem *> heap;
    int ProblemNum;
};

#endif