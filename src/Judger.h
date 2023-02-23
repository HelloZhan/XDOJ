#ifndef JUDGER_H
#define JUDGER_H
#include <jsoncpp/json/json.h>
#include <string>

class Judger
{
public:
    Judger();
    Json::Value Run(Json::Value &runjson);

private:
    bool Init(Json::Value &initjson);
    bool Compiler();
    bool RunProgram();
    Json::Value Done();

private:
    std::string m_runid;
    std::string m_problemid;
    int m_testnum;

    std::string m_result;
    std::string m_command;
};

#endif