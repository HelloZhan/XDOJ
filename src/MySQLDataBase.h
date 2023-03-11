#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H
#include <mysql/mysql.h>
#include <string>
#include <jsoncpp/json/json.h>

class MyDB
{
public:
    // 局部静态特性的方式实现单实例
    static MyDB &GetInstance();

    bool InitDB(std::string host, std::string user, std::string pwd, std::string dbname);
    bool ExeSQL(std::string sql);

    // 功能：将（题目，题目描述，测试数量）保存到数据库中
    int InsertProblem(std::string title, std::string description, int testnum);

    // 功能：取出题库数据
    Json::Value getAllProblemInfo();

    // 功能：普通分页查询题库信息
    Json::Value SelectProblemSetInfo(Json::Value &queryjson);

    // 功能：分页查询状态记录信息
    Json::Value SelectStatusRecordInfo(Json::Value &queryjson);

    // 功能：插入状态信息并返回ID
    std::string InsertStatusRecordInfo(Json::Value &insertjson);

    // 功能：修改状态信息（根据ID）
    bool UpdateStatusRecordInfo(Json::Value &updatejson);

    // 功能：修改题目的信息
    bool UpdateProblemStatusNum(std::string &problemid, std::string &statusnum);

private:
    MyDB();
    ~MyDB();

private:
    MYSQL *mysql;
    MYSQL_ROW row;
    MYSQL_RES *result;
    MYSQL_FIELD *field;
};
#endif
