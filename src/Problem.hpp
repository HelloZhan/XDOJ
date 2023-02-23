#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
using namespace std;
class Problem
{
public:
    Problem() {}
    bool init(Json::Value &jsonvalue)
    {
        m_id = jsonvalue["Pid"].asString();
        m_title = jsonvalue["Title"].asString();
        m_description = jsonvalue["Description"].asString();
        m_testnum = stoi(jsonvalue["TestNum"].asString());
        m_submitnum = stoi(jsonvalue["SubmitNum"].asString());
        m_acceptnum = stoi(jsonvalue["AcceptNum"].asString());
    }
    Json::Value getinfo()
    {
        Json::Value resjson;
        resjson["id"] = m_id;
        resjson["title"] = m_title;
        resjson["submitnum"] = m_submitnum;
        resjson["acceptnum"] = m_acceptnum;
        return resjson;
    }
    int getTestNum()
    {
        return m_testnum;
    }
    std::string getproblemdescription()
    {
        return m_description;
    }

private:
    std::string m_id;
    std::string m_title;
    std::string m_description;
    int m_testnum;
    int m_submitnum;
    int m_acceptnum;
};