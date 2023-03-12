#ifndef MONGODATABASE_H
#define MONGODATABASE_H
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pipeline.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <jsoncpp/json/json.h>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
class MoDB
{
public:
    // 局部静态特性的方式实现单实例
    static MoDB &GetInstance();

    // 初始化
    bool InitDB();
    // ------------------用户表User---------------------------
    /*
        功能：注册用户
        前端传入
        Json(NickName,Account,PassWord,PersonalProfile,School,Major)
        后端传出
        Json(Result,Reason)
    */
    Json::Value RegisterUser(Json::Value &registerjson);

    /*
        功能：登录用户
        前端传入
        Json(Account,PassWord)
        后端传出
        Json(Result,Reason,_id,NickName,Avatar)
    */
    Json::Value LoginUser(Json::Value &loginjson);

    // ----------------------题目表Problem--------------------
    /*
        功能：获取全部题目信息（是ProblemSet类进行初始化）
        Json(_id,Title,TimeLimit,MemoryLimit,Description,JudgeNum)
    */
    Json::Value getAllProblem();

    /*
        功能：分页获取题目列表（包含查询条件，暂时未添加）
        前端传入
        Json(QueryType,Page,PageSize,MatchString)
        后端传出
        Json(ProblemId,Title,SubmitNum,CENum,ACNum,WANum,TLENum,MLENum),TotalNum
    */
    Json::Value getProblemSet(Json::Value &queryjson);
    // Disscuss 讨论

    // 查询所有讨论
    Json::Value getAllDiscuss();

    Json::Value getFatherComment(Json::Value &queryjson);

    Json::Value getSonComment(Json::Value &queryjson);

    Json::Value getAllArticle();

    // 评论
    Json::Value InsertFatherComment(Json::Value &insertjson);

    Json::Value InsertSonComment(Json::Value &insertjson);

private:
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::client client{mongocxx::uri{}};
    mongocxx::database db;

    mongocxx::collection usercoll;
    mongocxx::collection problemcoll;
    mongocxx::collection articlecoll;
    mongocxx::collection discusscoll;
    mongocxx::collection commentcoll;
    MoDB();
    ~MoDB();
};

#endif