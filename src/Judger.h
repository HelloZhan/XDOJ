#ifndef JUDGER_H
#define JUDGER_H
#include <jsoncpp/json/json.h>
#include <string>

// #define PJ "Pending & Judging"
// #define CE "Compile Error"
// #define AC "Accepted"
// #define WA "Wrong Answer"
// #define RE "Runtime Error"
// #define TLE "Time Limit Exceeded"
// #define MLE "Memory Limit Exceeded"
// #define SE "System Error"

#define PJ 0
#define CE 1
#define AC 2
#define WA 3
#define RE 4
#define TLE 5
#define MLE 6
#define SE 7

class Judger
{
public:
    Judger();
    // 传入: Json(SubmitId,ProblemId,JudgeNum,Code,Language,TimeLimit,MemoryLimit,Length)
    // 返回：Json(Result,Reason,RunTime,RunMemory)
    /*
        功能：判题函数
        传入数据：Json(SubmitId,ProblemId,JudgeNum,Code,Language,TimeLimit,MemoryLimit)
        传出数据：Json(Status,RunTime,RunMemory,Length,CompilerInfo,
            TestInfo(Status,StandardOutput,PersonalOutput,RunTime,RunMemory))
    */
    Json::Value Run(Json::Value &runjson);

private:
    // 数据初始化
    bool Init(Json::Value &initjson);

    // -----C-----
    bool CompilerC(); // 编译C

    bool CompilerCpp(); // 编译C++

    bool CompilerGo(); // 编译Go

    bool RunProgramC_Cpp(); // 运行C或者C++

    bool RunProgramGo(); // 运行Go
    // 返回结果
    Json::Value Done();

private:
    std::string m_submitid;  // 运行ID
    std::string m_problemid; // 题目ID
    int m_judgenum;          // 测试用例数目
    std::string m_code;

    std::string m_result;   // 运行结果
    std::string m_reason;   // 错误原因
    std::string m_command;  // 命令（中间变量）
    std::string m_language; // 测评语言

    std::string m_length; // 文件长度

    int m_timelimit;    // 时间限制
    long m_memorylimit; // 空间限制

    int m_maxtimelimit;   // 最大时间限制
    int m_maxmemorylimie; // 最大空间限制

    int m_runtime;    // 运行时间
    long m_runmemory; // 运行空间
};

#endif