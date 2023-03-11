#include "MySQLDataBase.h"
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
    string sql = "select ProblemId,Title,Description,JudgeNum from Problem;";

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

            resJson["Array"].append(info);
        }
        mysql_free_result(result);
    }
    printf("MySQL getAllProblemInfo finish!!\n");
    return resJson;
}

Json::Value MyDB::SelectProblemSetInfo(Json::Value &queryjson)
{
    printf("MySQL SelectProblemSetInfo!!\n");
    string querytype = queryjson["QueryType"].asString();
    string limit = queryjson["PageSize"].asString();
    string offest = to_string((stoi(queryjson["Page"].asString()) - 1) * stoi(limit));
    string sql;
    if (querytype == "common") // 普通分页查询
    {
        sql = "select SQL_CALC_FOUND_ROWS ProblemId,Title,JudgeNum,SubmitNum,CENum,ACNum,WANum,TLENum,MLENum from Problem order by ProblemId limit " + offest + "," + limit + ";";
    }
    else if (querytype == "fuzzy") // 模糊匹配查询
    {
    }

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
    printf("MySQL SelectProblemSetInfo finish!!\n");
    return resJson;
}
Json::Value MyDB::SelectStatusRecordInfo(Json::Value &queryjson)
{
    printf("MySQL getStatusRecordInfo!!\n");
    string querytype = queryjson["QueryType"].asString();
    string limit = queryjson["PageSize"].asString();
    string offest = to_string((stoi(queryjson["Page"].asString()) - 1) * stoi(limit));
    string sql;
    if (querytype == "all")
    {
        sql = "select SQL_CALC_FOUND_ROWS SubmitId,UserNickName,ProblemTitle,Status,RunTime,RunMemory,Length,Language,SubmitTime,Code from StatusRecord order by SubmitId desc limit " + offest + "," + limit + ";";
    }
    else if (querytype == "user")
    {
    }
    else if (querytype == "problem")
    {
    }
    else if (querytype == "userproblem")
    {
    }

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
            info["SubmitId"] = row[0];
            info["UserNickName"] = row[1];
            info["ProblemTitle"] = row[2];
            info["Status"] = row[3];
            info["RunTime"] = row[4];
            info["RunMemory"] = row[5];
            info["Length"] = row[6];
            info["Language"] = row[7];
            info["SubmitTime"] = row[8];
            info["Code"] = row[9];

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
    printf("MySQL getStatusRecordInfo finish!!\n");
    return resJson;
}
// 功能：插入状态信息并返回ID
std::string MyDB::InsertStatusRecordInfo(Json::Value &insertjson)
{
    // 传入：Json(ProblemId,UserId,UserNickName,ProblemTitle,Language,Code);
    printf("InsertStatusRecordInfo function!\n");
    int problemid = stoi(insertjson["ProblemId"].asString());
    int userid = stoi(insertjson["UserId"].asString());
    string usernickname = insertjson["UserNickName"].asString();
    string problemtitle = insertjson["ProblemTitle"].asString();
    string language = insertjson["Language"].asString();
    string code = insertjson["Code"].asString();

    string sql = "insert into StatusRecord value(null," + to_string(problemid) + "," + to_string(userid) + ",\" " + usernickname + " \",\" " + problemtitle + " \",\"Pending \",\"0MS\",\" 0MB \",\"0KB\",\" " + language + " \",default,\" " + code + " \");";

    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    // 获取执行成功的Id
    sql = "SELECT LAST_INSERT_ID();";
    int submitid = 0;
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
                    submitid = 1;
                else
                {
                    string k = row[j];
                    submitid = stoi(k);
                }
            }
        }
        mysql_free_result(result);
    }
    printf("submitid = %d\n", submitid);
    // 返回：SubmitId
    return to_string(submitid);
}

// 功能：修改状态信息（根据ID）
bool MyDB::UpdateStatusRecordInfo(Json::Value &updatejson)
{
    // 传入：Json(SubmitId,Status,RunTime,RunMemory,Length)
    int submitid = stoi(updatejson["SubmitId"].asString());
    string status = updatejson["Status"].asString();
    string runtime = updatejson["RunTime"].asString();
    string runmemory = updatejson["RunMemory"].asString();
    string length = updatejson["Length"].asString();
    string sql = "update StatusRecord set Status = \" " + status + " \",RunTime = \" " + runtime + " \",RunMemory =\" " + runmemory + " \",Length = \"" + length + "\" where SubmitId = " + to_string(submitid) + ";";

    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    return true;
}

bool MyDB::UpdateProblemStatusNum(string &problemid, string &statusnum)
{
    string sql = "update Problem set SubmitNum = SubmitNum + 1 , " + statusnum + " = " + statusnum + "+ 1 where ProblemId = " + problemid + ";";
    if (mysql_query(mysql, sql.data()))
    {
        printf("query fail: %s\n", mysql_error(mysql));
        exit(1);
    }
    return true;
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