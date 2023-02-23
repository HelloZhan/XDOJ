#include "Judger.h"
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <iostream>
extern "C"
{
#include "runner.h"
}
using namespace std;
Judger::Judger()
{
}
Json::Value Judger::Run(Json::Value &runjson)
{
    if (!Init(runjson))
        return Done();
    if (!Compiler())
        return Done();
    if (!RunProgram())
        return Done();

    return Done();
}

bool Judger::Init(Json::Value &initjson)
{
    // 初始化数据
    string code = initjson["code"].asString();
    m_runid = initjson["runid"].asString();
    m_problemid = initjson["problemid"].asString();
    m_testnum = initjson["testnum"].asInt();
    m_result = "pedding";

    // 创建中间文件夹
    m_command = "mkdir " + m_runid;
    system(m_command.data());

    // TODO:检查代码是否有敏感词

    // 将代码输出到文件中
    ofstream outfile;
    m_command = "./" + m_runid + "/main.cpp";
    outfile.open(m_command.data());
    outfile << code.data();
    outfile.close();

    return true;
}
bool Judger::Compiler()
{
    m_command = "timeout 10 g++ ./" + m_runid + "/main.cpp -o ./" + m_runid + "/main.out -O2 -std=c++11 2>./" + m_runid + "/errorce.txt";
    system(m_command.data());
    return true;
}
bool Judger::RunProgram()
{
    // 创建结构体
    struct config conf;
    struct result res;
    conf.max_cpu_time = 1000;
    conf.max_real_time = 2000;
    conf.max_memory = 128 * 1024 * 1024;
    conf.max_process_number = 200;
    conf.max_output_size = 10000;
    conf.max_stack = 32 * 1024 * 1024;
    string exe_path = "./" + m_runid + "/main.out";
    conf.exe_path = (char *)exe_path.data();
    string error_path = "./" + m_runid + "/error.out";
    conf.error_path = (char *)error_path.data();
    string log_path = "./" + m_runid + "/judger.log";
    conf.log_path = (char *)log_path.data();
    conf.seccomp_rule_name = (char *)"c_cpp";
    conf.uid = 0;
    conf.gid = 0;
    memset(conf.args, 0, sizeof(conf.args));
    memset(conf.env, 0, sizeof(conf.env));
    for (int i = 1; i <= m_testnum; i++)
    {
        string index = to_string(i);
        string input_path = "../problem/" + m_problemid + "/" + index + ".in";
        conf.input_path = (char *)input_path.data();
        string output_path = "./" + m_runid + "/" + index + ".out";
        conf.output_path = (char *)output_path.data();

        // 执行程序
        run(&conf, &res);
        printf("cpu time is %d,real time is %d,memory is %ld,signal is %d,result is %d\n", res.cpu_time, res.real_time, res.memory, res.signal, res.result);

        // 判断结果
        if (res.result == 0)
        {
            // 获取标准答案
            ifstream infile1;
            m_command = "../problem/" + m_problemid + "/" + index + ".out";
            infile1.open(m_command.data());
            string standardanswer;
            infile1 >> standardanswer;

            // 获取计算答案
            ifstream infile2;
            m_command = "./" + m_runid + "/" + index + ".out";
            infile2.open(m_command.data());
            string calculateanswer;
            infile2 >> calculateanswer;
            printf("standardanswer = %s\n", standardanswer.data());
            printf("calculateanswer = %s\n", calculateanswer.data());

            if (strcmp(standardanswer.data(), calculateanswer.data()) != 0)
            {
                m_result = "WA";
                cout << "答案错误" << endl;
                return false;
            }
            printf("答案正确\n");
        }
        else
        {
            m_result = "SE";
            cout << "系统错误" << endl;
            return false;
        }
    }
    m_result = "AC";
    return true;
}
Json::Value Judger::Done()
{
    Json::Value resjson;
    resjson["result"] = m_result;

    // 删除中间文件夹
    m_command = "rm -rf ./" + m_runid;
    system(m_command.data());

    return resjson;
}