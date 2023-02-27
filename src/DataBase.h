#ifndef DATABASE_H
#define DATABASE_H
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

    Json::Value getAllUserInfo();

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
