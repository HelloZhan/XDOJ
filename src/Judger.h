#ifndef JUDGER_H
#define JUDGER_H
#include <jsoncpp/json/json.h>
#include <string>

#define PJ "Pending & Judging"
#define CE "Compile Error"
#define AC "Accepted"
#define WA "Wrong Answer"
#define RE "Runtime Error"
#define TLE "Time Limit Exceeded"
#define MLE "Memory Limit Exceeded"
#define SE "System Error"

class Judger
{
public:
    Judger();

    // 接口：Json(runid,problemid,judgenum,code,language,timelimit,memorylimit)
    // 返回：Json(result,reason,language)
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
    std::string m_reason;
    std::string m_command;
    std::string m_language;

    int m_timelimit;
    long m_memorylimit;
    int m_maxtimelimit;
    int m_maxmemorylimie;
};

#endif