#pragma once

#include <iostream>
#include <map>
#include <string>
#include "Problem.hpp"
#include <string.h>
#include "DataBase.h"
using namespace std;

class ProblemSet
{
public:
    ProblemSet() {}
    void init()
    {
        MyDB mydb;
        mydb.InitDB("192.168.49.130", "root", "1", "XDOJ");
        Json::Value jsonvalue = mydb.getAllProblemInfo();
        ProblemNum = (int)jsonvalue["Array"].size();
        for (int i = 0; i < ProblemNum; i++)
        {
            Problem *tmp = new Problem();
            tmp->init(jsonvalue["Array"][i]);
            heap[jsonvalue["Array"][i]["Pid"].asString()] = tmp;
        }
    }
    std::string getproblemdescription(std::string id)
    {
        return heap[id]->getproblemdescription();
    }
    // 获取题库数据
    Json::Value getproblemset()
    {
        Json::Value resjson;
        for (auto v : heap)
        {
            resjson.append(v.second->getinfo());
        }
        return resjson;
    }
    int getproblemtestnum(std::string id)
    {
        return heap[id]->getTestNum();
    }
    ~ProblemSet()
    {
        for (auto v : heap)
        {
            delete v.second;
        }
        heap.clear();
    }

private:
    std::map<string, Problem *> heap;
    int ProblemNum;
};