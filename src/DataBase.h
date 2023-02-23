#ifndef DATABASE_H
#define DATABASE_H
#include <mysql/mysql.h>
#include <string>
#include <jsoncpp/json/json.h>

class MyDB
{
public:
    MyDB();
    ~MyDB();
    bool InitDB(std::string host, std::string user, std::string pwd, std::string dbname);
    bool ExeSQL(std::string sql);

    // 功能：将（题目，题目描述，测试数量）保存到数据库中
    int InsertProblem(std::string title, std::string description, int testnum);

    // 功能：将所有数据拿出来
    Json::Value getAllProblemInfo();

private:
    MYSQL *mysql;
    MYSQL_ROW row;
    MYSQL_RES *result;
    MYSQL_FIELD *field;
};
#endif
