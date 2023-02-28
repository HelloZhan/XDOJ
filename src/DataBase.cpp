#include "DataBase.h"
#include <string>
#include <iostream>
using namespace std;

MyDB &MyDB::GetInstance()
{
    static MyDB mydb;
    return mydb;
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
int MyDB::InsertProblem(string title, string description, int judgenum)
{
    printf("InsertProblem function!\n");
    string sql = "select max(ProblemId) from Problem;";
    int ProblemId = 0;
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
                    ProblemId = 1;
                else
                {
                    string k = row[j];
                    ProblemId = stoi(k);
                }
            }
        }
        mysql_free_result(result);
    }
    ProblemId++;
    printf("ProblemId = %d\n", ProblemId);
    if (ProblemId == 0)
        return 0;

    sql = "insert into Problem value(" + to_string(ProblemId) + ", \"" + title + "\",\"" + description + "\"," + to_string(judgenum) + ",0,0,0,0,0,0);";
    printf("%s\n", sql.data());
    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    printf("InsertProblem finish!\n");
    return ProblemId;
}

Json::Value MyDB::getAllProblemInfo()
{
    printf("MySQL getAllProblemInfo!!\n");
    string sql = "select ProblemId, Title,Description,JudgeNum,SubmitNum,CENum,ACNum,WANum,TLENum,MLENum from Problem;";

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
            info["ProblemId"] = row[0];
            info["Title"] = row[1];
            info["Description"] = row[2];
            info["JudgeNum"] = row[3];
            info["SubmitNum"] = row[4];
            info["CENum"] = row[5];
            info["ACNum"] = row[6];
            info["WANum"] = row[7];
            info["TLENum"] = row[8];
            info["MLENum"] = row[9];

            resJson["Array"].append(info);
        }
        mysql_free_result(result);
    }
    printf("MySQL getAllProblemInfo finish!!\n");
    return resJson;
}
Json::Value MyDB::getAllUserInfo()
{
    printf("MySQL getAllUserInfo!!\n");
    string sql = "select UserId, Avator,NickName,Account,Password from User;";

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
            info["UserId"] = row[0], info["Avator"] = row[1], info["NickName"] = row[2], info["Account"] = row[3], info["Password"] = row[4];
            // cout << row[0] << ' ' << row[1] << ' ' << row[3] << ' ' << row[4] << ' ' << row[5] << endl;
            resJson["Array"].append(info);
        }
        mysql_free_result(result);
    }
    printf("MySQL getAllUserInfo finish!!\n");
    return resJson;
}

Json::Value MyDB::getAllProblemSetInfoByCommon(string offest, string limit)
{
    printf("MySQL getAllProblemSetInfoByCommon!!\n");
    string sql = "select SQL_CALC_FOUND_ROWS ProblemId,Title,JudgeNum,SubmitNum,CENum,ACNum,WANum,TLENum,MLENum from Problem order by ProblemId limit " + offest + "," + limit + ";";

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
            info["ProblemId"] = row[0];
            info["Title"] = row[1];
            info["JudgeNum"] = row[2];
            info["SubmitNum"] = row[3];
            info["CENum"] = row[4];
            info["ACNum"] = row[5];
            info["WANum"] = row[6];
            info["TLENum"] = row[7];
            info["MLENum"] = row[8];

            resJson["Array"].append(info);
        }
        mysql_free_result(result);
    }
    // 获取总共有多少条数
    sql = "SELECT FOUND_ROWS()";
    string totalsize = "0";
    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    else
    {
        /*获取结果集*/
        result = mysql_store_result(mysql);

        row = mysql_fetch_row(result);
        totalsize = row[0];

        mysql_free_result(result);
    }
    resJson["TotalNum"] = totalsize;
    printf("MySQL getAllProblemSetInfoByCommon finish!!\n");
    return resJson;
}

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