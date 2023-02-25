#include "DataBase.h"
#include <string>
using namespace std;

MyDB::MyDB()
{
    mysql = mysql_init(NULL);
    if (mysql == NULL)
    {
        printf("Error: %s\n", mysql_error(mysql));
        exit(-1);
    }
}

MyDB::~MyDB()
{
    if (!mysql)
    {
        mysql_close(mysql);
    }
}

bool MyDB::InitDB(string host, string user, string pwd, string dbname)
{
    /*连接数据库*/
    if (!mysql_real_connect(mysql, host.c_str(), user.c_str(), pwd.c_str(), dbname.c_str(), 0, NULL, 0))
    {
        printf("Error: %s\n", mysql_error(mysql));
        exit(-1);
    }
    // 避免中文乱码
    mysql_query(mysql, "set names utf8");
    return true;
}

bool MyDB::ExeSQL(string sql)
{
    /*执行失败*/

    if (mysql_query(mysql, sql.c_str()))
    {
        printf("Error: %s\n", mysql_error(mysql));
        exit(1);
    }
    else
    {
        /*获取结果集*/
        result = mysql_store_result(mysql);

        int rownum = mysql_num_rows(result);
        int fieldnum = mysql_num_fields(result);
        for (int i = 0; i < rownum; i++)
        {
            row = mysql_fetch_row(result);
            if (row <= 0)
                break;
            for (int j = 0; j < fieldnum; j++)
            {
                printf("%s\t\t", row[j]);
            }
            printf("\n");
        }
        mysql_free_result(result);
    }
    return true;
}
int stringtoint(string sn)
{
    int res = 0;
    for (auto k : sn)
    {
        res = res * 10 + k - '0';
    }
    return res;
}
int MyDB::InsertProblem(string title, string description, int testnum)
{
    printf("InsertProblem function!\n");
    string sql = "select max(Pid) from ProblemSet;";
    int Pid = 0;
    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    else
    {
        /*获取结果集*/
        result = mysql_store_result(mysql);

        int rownum = mysql_num_rows(result);
        int fieldnum = mysql_num_fields(result);
        for (int i = 0; i < rownum; i++)
        {
            row = mysql_fetch_row(result);
            if (row <= 0)
                break;
            for (int j = 0; j < fieldnum; j++)
            {
                if (row[j] == nullptr)
                    Pid = 1;
                else
                {
                    string k = row[j];
                    Pid = stringtoint(k);
                }
            }
        }
        mysql_free_result(result);
    }
    Pid++;
    printf("Pid = %d\n", Pid);
    if (Pid == 0)
        return 0;

    sql = "insert into ProblemSet value(" + to_string(Pid) + ", \"" + title + "\",\"" + description + "\"," + to_string(testnum) + ",0,0);";
    printf("%s\n", sql.data());
    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    printf("InsertProblem finish!\n");
    return Pid;
}

Json::Value MyDB::getAllProblemInfo()
{
    printf("getAllProblemInfo!!\n");
    string sql = "select Pid, Title,Description,JudgeNum,SubmitNum,AcceptNum from ProblemSet;";

    Json::Value resJson;
    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    else
    {
        /*获取结果集*/
        result = mysql_store_result(mysql);

        int rownum = mysql_num_rows(result);
        int fieldnum = mysql_num_fields(result);
        for (int i = 0; i < rownum; i++)
        {
            row = mysql_fetch_row(result);
            if (row <= 0)
                break;
            Json::Value info;
            info["Pid"] = row[0], info["Title"] = row[1], info["Description"] = row[2], info["JudgeNum"] = row[3], info["SubmitNum"] = row[4], info["AcceptNum"] = row[5];
            // cout << row[0] << ' ' << row[1] << ' ' << row[3] << ' ' << row[4] << ' ' << row[5] << endl;
            resJson["Array"].append(info);
        }
        mysql_free_result(result);
    }
    printf("getAllProblemInfo finish!!\n");
    return resJson;
}