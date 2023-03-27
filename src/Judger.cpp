#include "Judger.h"
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

extern "C"
{
#include "../include/runner.h"
}

using namespace std;
Judger::Judger()
{
}
Json::Value Judger::Run(Json::Value &runjson)
{
    if (!Init(runjson))
        return Done();
    // 编译
    if (m_language == "C")
    {
        if (!CompilerC())
            return Done();
    }
    else if (m_language == "C++")
    {
        if (!CompilerCpp())
            return Done();
    }
    else if (m_language == "Go")
    {
        if (!CompilerGo())
            return Done();
    }
    // 运行
    if (m_language == "C" || m_language == "C++")
    {
        if (!RunProgramC_Cpp())
            return Done();
    }
    else if (m_language == "Go")
    {
        if (!RunProgramGo())
            return Done();
    }

    return Done();
}

bool Judger::Init(Json::Value &initjson)
{
    // 初始化数据
    // Json(SubmitId,ProblemId,JudgeNum,Code,Language,TimeLimit,MemoryLimit)
    m_submitid = initjson["SubmitId"].asString();
    m_problemid = initjson["ProblemId"].asString();
    m_code = initjson["Code"].asString();

    m_judgenum = initjson["JudgeNum"].asInt();
    m_timelimit = initjson["TimeLimit"].asInt();
    m_memorylimit = initjson["MemoryLimit"].asLargestInt() * 1024 * 1024;
    m_language = initjson["Language"].asString();
    m_maxtimelimit = m_timelimit * 2;
    m_maxmemorylimie = m_memorylimit * 2;
    m_result = PJ;
    m_reason = "";
    m_runmemory = 0;
    m_runtime = 0;
    m_isspj = false;

    RUN_PATH = "./" + m_submitid + "/";
    DATA_PATH = "../../problemdata/" + m_problemid + "/";
    // 获取信息
    m_resjson.clear();

    // 创建中间文件夹
    m_command = "mkdir " + m_submitid;
    if (system(m_command.data()) == -1)
    {
        m_result = SE;
        return false;
    }

    // 将代码输出到文件中
    ofstream outfile;
    if (m_language == "C")
    {
        m_command = RUN_PATH + "main.c";
    }
    else if (m_language == "C++")
    {
        m_command = RUN_PATH + "main.cpp";
    }
    else if (m_language == "Go")
    {
        m_command = RUN_PATH + "main.go";
    }
    outfile.open(m_command.data());
    if (!outfile.is_open())
    {
        m_result = SE;
        return false;
    }
    outfile << m_code.data();
    outfile.close();

    // 编译spj文件
    CompilerSPJ();

    struct stat statbuf;
    stat(m_command.data(), &statbuf);
    m_length = to_string((int)statbuf.st_size / 128) + "KB";

    return true;
}

bool Judger::CompilerSPJ()
{
    m_command = DATA_PATH + "spj.cpp";
    // 如果不存在
    if (access(m_command.data(), F_OK) == -1)
        return true;

    // 编译SPJ文件
    m_command = "timeout 10 g++ " + DATA_PATH + "spj.cpp -o spj -O2 -std=c++14";
    if (system(m_command.data()) == -1)
    {
        m_result = SE;
        return false;
    }
    m_isspj = true;
    return true;
}

bool Judger::CompilerC()
{
    // 进行gcc编译
    m_command = "timeout 10 gcc " + RUN_PATH + "main.c -fmax-errors=3 -o " + RUN_PATH + "main -O2 -std=c11 2>" + RUN_PATH + "compileinfo.txt";
    if (system(m_command.data()) == -1)
    {
        m_result = SE;
        return false;
    }

    m_command = RUN_PATH + "main";
    // 编译失败
    if (access(m_command.data(), F_OK) == -1)
    {
        // 返回编译失败原因
        ifstream infile;

        m_command = RUN_PATH + "compileinfo.txt";
        infile.open(m_command.data());
        // 读取全部信息
        string reason((istreambuf_iterator<char>(infile)),
                      (istreambuf_iterator<char>()));

        m_reason = reason;
        m_result = CE;
        infile.close();
        return false;
    }
    return true;
}

bool Judger::CompilerCpp()
{
    // 进行g++编译
    m_command = "timeout 10 g++ " + RUN_PATH + "main.cpp -fmax-errors=3 -o " + RUN_PATH + "main -O2 -std=c++11 2>" + RUN_PATH + "compileinfo.txt";
    if (system(m_command.data()) == -1)
    {
        m_result = SE;
        return false;
    }

    m_command = RUN_PATH + "main";
    // 编译失败
    if (access(m_command.data(), F_OK) == -1)
    {
        // 返回编译失败原因
        ifstream infile;

        m_command = RUN_PATH + "compileinfo.txt";
        infile.open(m_command.data());
        // 读取全部信息
        string reason((istreambuf_iterator<char>(infile)),
                      (istreambuf_iterator<char>()));

        m_reason = reason;
        m_result = CE;
        infile.close();
        return false;
    }
    return true;
}

bool Judger::CompilerGo()
{
    // 进行g++编译
    m_command = "go build -o " + RUN_PATH + "main " + RUN_PATH + "main.go 2>" + RUN_PATH + "compileinfo.txt";
    if (system(m_command.data()) == -1)
    {
        m_result = SE;
        return false;
    }

    m_command = RUN_PATH + "main";
    // 编译失败
    if (access(m_command.data(), F_OK) == -1)
    {
        // 返回编译失败原因
        ifstream infile;

        m_command = RUN_PATH + "compileinfo.txt";
        infile.open(m_command.data());
        // 读取全部信息
        string reason((istreambuf_iterator<char>(infile)),
                      (istreambuf_iterator<char>()));

        m_reason = reason;
        m_result = CE;
        infile.close();
        return false;
    }
    return true;
}
bool Judger::RunProgramC_Cpp()
{
    // 创建结构体
    struct config conf;
    struct result res;

    conf.max_cpu_time = m_timelimit;
    conf.max_real_time = m_maxtimelimit;
    conf.max_memory = m_maxmemorylimie;

    // conf.max_cpu_time = 2000;
    // conf.max_real_time = 4000;
    // conf.max_memory = 128 * 1024 * 1024;
    conf.max_process_number = 200;
    conf.max_output_size = 10000;
    conf.max_stack = 32 * 1024 * 1024;
    string exe_path = RUN_PATH + "main";
    conf.exe_path = (char *)exe_path.data();
    string error_path = RUN_PATH + "error.out";
    conf.error_path = (char *)error_path.data();
    string log_path = RUN_PATH + "judger.log";
    conf.log_path = (char *)log_path.data();
    conf.seccomp_rule_name = (char *)"c_cpp";
    conf.uid = 0;
    conf.gid = 0;
    memset(conf.args, 0, sizeof(conf.args));
    memset(conf.env, 0, sizeof(conf.env));

    // 根据测试数量进行判定
    for (int i = 1; i <= m_judgenum; i++)
    {
        Json::Value testinfo;
        string index = to_string(i);
        string input_path = DATA_PATH + index + ".in";
        conf.input_path = (char *)input_path.data();
        string output_path = RUN_PATH + index + ".out";
        conf.output_path = (char *)output_path.data();

        // 执行程序
        run(&conf, &res);
        printf("cpu time is %d,real time is %d,memory is %ld,signal is %d,result is %d\n", res.cpu_time, res.real_time, res.memory, res.signal, res.result);

        JudgmentResult(&res, index);
    }
    return true;
}

bool Judger::RunProgramGo()
{
    // 创建结构体
    struct config conf;
    struct result res;

    conf.max_cpu_time = m_timelimit;
    conf.max_real_time = m_maxtimelimit;
    conf.max_memory = m_maxmemorylimie;

    // conf.max_cpu_time = 2000;
    // conf.max_real_time = 4000;
    // conf.max_memory = 128 * 1024 * 1024;
    conf.max_process_number = 200;
    conf.max_output_size = 10000;
    conf.max_stack = 32 * 1024 * 1024;
    string exe_path = RUN_PATH + "main";
    conf.exe_path = (char *)exe_path.data();
    string error_path = RUN_PATH + "error.out";
    conf.error_path = (char *)error_path.data();
    string log_path = RUN_PATH + "judger.log";
    conf.log_path = (char *)log_path.data();
    conf.seccomp_rule_name = (char *)"golang";
    conf.uid = 0;
    conf.gid = 0;
    memset(conf.args, 0, sizeof(conf.args));
    memset(conf.env, 0, sizeof(conf.env));
    conf.env[0] = (char *)"LANG=en_US.UTF-8";
    conf.env[1] = (char *)"LANGUAGE=en_US:en";
    conf.env[2] = (char *)"LC_ALL=en_US.UTF-8";

    // 根据测试数量进行判定
    for (int i = 1; i <= m_judgenum; i++)
    {
        string index = to_string(i);
        string input_path = DATA_PATH + index + ".in";
        conf.input_path = (char *)input_path.data();
        string output_path = RUN_PATH + index + ".out";
        conf.output_path = (char *)output_path.data();

        // 执行程序
        run(&conf, &res);
        printf("cpu time is %d,real time is %d,memory is %ld,signal is %d,result is %d\n", res.cpu_time, res.real_time, res.memory, res.signal, res.result);

        JudgmentResult(&res, index);
    }
    return true;
}

bool Judger::JudgmentResult(struct result *res, string &index)
{
    Json::Value testinfo; // 保存本次结果
    // 获取最大时间和空间
    m_runtime = max(m_runtime, res->cpu_time);
    m_runmemory = max(m_runmemory, res->memory);

    // 获取运行时间和运行内存的数据
    testinfo["RunTime"] = to_string(res->cpu_time) + "MS";
    testinfo["RunMemory"] = to_string(res->memory / 1024 / 1024) + "MB";

    // 判断结果
    if (res->result == 0)
    {
        // 获取标准答案
        ifstream infile1;
        string datapath = DATA_PATH + index + ".out";
        infile1.open(datapath.data());
        string standardanswer((istreambuf_iterator<char>(infile1)),
                              (istreambuf_iterator<char>()));
        infile1.close();
        // 获取计算答案
        ifstream infile2;
        string runpath = RUN_PATH + index + ".out";
        infile2.open(runpath.data());
        string calculateanswer((istreambuf_iterator<char>(infile2)),
                               (istreambuf_iterator<char>()));
        infile2.close();
        printf("standardanswer = %s\n", standardanswer.data());
        printf("calculateanswer = %s\n", calculateanswer.data());

        testinfo["StandardOutput"] = standardanswer;
        testinfo["PersonalOutput"] = calculateanswer;

        // 判断是否超出时间限制
        if (res->cpu_time > m_timelimit)
        {
            m_result = TLE;
            testinfo["Status"] = TLE;
        }
        else if (res->memory > m_memorylimit)
        {
            m_result = MLE;
            testinfo["Status"] = MLE;
        }
        else if (m_isspj) // SPJ判断
        {
            cout << "SPJ判断" << endl;
            m_command = "./spj " + datapath + " " + runpath;
            testinfo["Status"] = AC;
            if (system(m_command.data()) != 0) // 如果失败
            {
                testinfo["Status"] = WA;
                m_reason = "standardanswer = " + standardanswer + ",calculateanswer = " + calculateanswer + ";";
            }
        }
        else // 普通比较
        {
            testinfo["Status"] = AC;
            // 比较答案 TODO：简易版，后续完善（添加去除行末空格）
            if (strcmp(standardanswer.data(), calculateanswer.data()) != 0)
            {
                testinfo["Status"] = WA;
                m_result = WA;
                m_reason = "standardanswer = " + standardanswer + ",calculateanswer = " + calculateanswer + ";";
            }
        }
    }
    else if (res->result == 1) // CPU_TIME_LIMIT_EXCEEDED
    {
        m_result = TLE;
        testinfo["Status"] = TLE;
    }
    else if (res->result == 2) // REAL_TIME_LIMIT_EXCEEDED
    {
        m_result = TLE;
        testinfo["Status"] = TLE;
    }
    else if (res->result == 3) // MEMORY_LIMIT_EXCEEDED
    {
        m_result = MLE;
        testinfo["Status"] = MLE;
    }
    else if (res->result == 4)
    {
        // 获取失败原因
        ifstream infile;
        m_command = RUN_PATH + "error.out";
        infile.open(m_command.data());

        string reason((istreambuf_iterator<char>(infile)),
                      (istreambuf_iterator<char>()));

        m_reason = reason;
        m_result = RE;
        testinfo["Status"] = RE;
        infile.close();
    }
    m_resjson["TestInfo"].append(testinfo);
}

Json::Value Judger::Done()
{
    // 返回：Json(Result,Reason,RunTime,RunMemory)
    if (m_result == PJ)
        m_result = AC;

    m_resjson["SubmitId"] = m_submitid;
    m_resjson["Status"] = m_result;
    m_resjson["ComplierInfo"] = m_reason;
    m_resjson["RunTime"] = to_string(m_runtime) + "MS";
    m_resjson["RunMemory"] = to_string(int(m_runmemory / 1024 / 1024)) + "MB";
    m_resjson["Length"] = m_length;
    // 删除中间文件夹
    m_command = "rm -rf " + RUN_PATH;
    system(m_command.data());

    return m_resjson;
}