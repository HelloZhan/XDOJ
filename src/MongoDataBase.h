#ifndef MONGODATABASE_H
#define MONGODATABASE_H
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pipeline.hpp>
#include <mongocxx/pool.hpp>
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

    // +++++++++++++++++++++++用户表User+++++++++++++++++++++++++++++
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
        Json(Result,Reason,Info(_id,NickName,Avatar...))用户的全部信息，详情请见MongoDB集合表
    */
    Json::Value LoginUser(Json::Value &loginjson);

    /*
        功能：更新用户的题目信息，当用户提交代码时
        传入：Json(UserId,ProblemId,Status)
        传出：bool(如果AC是否向其添加)
    */
    bool UpdateUserProblemInfo(Json::Value &updatejson);

    /*
        功能：获取用户的Rank排名
        传入：Json(Page,PageSize)
        传出：Json(_id,Rank,Avatar,NickName,PersonalProfile,SubmitNum,ACNum),TotalNum
    */
    Json::Value SelectUserRank(Json::Value &queryjson);

    // ++++++++++++++++++++++++++题目表Problem+++++++++++++++++++++++++++++
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
        Json(ProblemId,Title,SubmitNum,CENum,ACNum,WANum,TLENum,MLENum,SENum),TotalNum
    */
    Json::Value getProblemSet(Json::Value &queryjson);

    /*
        功能：更新题目的状态数量
        传入：Json(ProblemId,Status)
        传出：bool
    */
    bool UpdateProblemStatusNum(Json::Value &updatejson);

    // ++++++++++++++++++++++++评测表StatusRecord+++++++++++++++++++++++++

    /*
        功能：向测评表插入一条待测评记录
        传入：Json(ProblemId,UserId,UserNickName,ProblemTitle,Language,Code)
        传出：SubmitId测评的ID
    */
    std::string InsertStatusRecord(Json::Value &insertjson);

    /*
        功能：更新测评记录并返回测评记录
        传入：Json(SubmitId,Status,RunTime,RunMemory,Length,ComplierInfo,
        TestInfo[(Status,StandardOutput,PersonalOutput,RunTime,RunMemory)])

        传出：Json() 测评记录
    */
    Json::Value UpdateStatusRecord(Json::Value &updatejson);

    /*
        功能：分页查询测评记录
        传入：Json(QueryType,PageSize,Page)
        传出：测评全部信息，详情请见MongoDB集合表
    */
    Json::Value SelectStatusRecord(Json::Value &queryjson);

    // +++++++++++++++++++++++++Disscuss 讨论表++++++++++++++++++++++++++++

    /*
        功能：添加讨论
        传入：Json(Title,Content,ParentId,UserId) 如果是父讨论ParentId=0
        传出：bool
    */
    bool InsertDiscuss(Json::Value &insertjson);

    /*
        功能：分页查询讨论
        传入：Json(ParentId,Page,PageSize)
        传出：Json(_id,Title,Views,Comments,CreateTime,User.Avatar,User.NickName)
    */
    Json::Value SelectDiscuss(Json::Value &queryjson);

    /*
        功能：查询讨论的详细内容，并且将其浏览量加一
        传入：Json(DiscussId)
        传出：Json(Content)
    */
    Json::Value SelectDiscussContent(Json::Value &queryjson);
    // 查询所有讨论
    Json::Value getAllDiscuss();

    // ++++++++++++++++++++++文章表 Article+++++++++++++++++++++
    Json::Value getAllArticle();
    // ++++++++++++++++++++++评论表 Comment+++++++++++++++++++++++
    /*
        功能：查询父评论
        传入：Json(ParentId,Skip,Limie,SonNum)
        传出：
        Json(ParentId,Content,Likes,CreateTime,Child_Total,
        User(Avatar,NickName),
        Child_Comments(_id,Content,Likes,CreateTime,User(Avatar,NickName)))
    */
    Json::Value getFatherComment(Json::Value &queryjson);

    /*
        功能：获取子评论
        传入：Json(ParentId,Skip,Limit)
        传出：Json(Child_Total,Child_Comments(_id,Content,Likes,CreateTime,User(NickName,Avatar)))
    */
    Json::Value getSonComment(Json::Value &queryjson);

    /*
        功能：插入父评论
        传入：Json(ParentId,Content,UserId)
        传出：Json(_id,CreateTime)
    */
    Json::Value InsertFatherComment(Json::Value &insertjson);

    /*
        功能：插入子评论
        传入：Json(ParentId,Content,UserId)
        传出：Json(_id,CreateTime)
    */
    Json::Value InsertSonComment(Json::Value &insertjson);

private:
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::uri uri{};           // 连接配置
    mongocxx::pool pool{uri};      // 连接池

    MoDB();
    ~MoDB();
};

#endif