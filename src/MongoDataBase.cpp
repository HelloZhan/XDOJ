#include "MongoDataBase.h"
#include <cstdint>
#include <iostream>
#include "./utils/snowflake.hpp"
#include <ctime>

using namespace std;
// 雪花算法
using snowflake_t = snowflake<1534832906275L, std::mutex>;
snowflake_t uuid;

// 获取时间
string GetTime()
{
    time_t t = time(nullptr);
    struct tm *now = localtime(&t);
    char str[50];
    strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", now);
    return (string)str;
}

MoDB &MoDB::GetInstance()
{
    static MoDB modb;
    return modb;
}

bool MoDB::InitDB()
{
    // 连接数据库
    db = client["XDOJ"];
    // 连接集合
    articlecoll = db["Article"];
    discusscoll = db["Discuss"];
    commentcoll = db["Comment"];
}

Json::Value MoDB::getAllDiscuss()
{
    Json::Reader reader;
    Json::Value resjson;
    mongocxx::cursor cursor = discusscoll.find({});
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        resjson.append(jsonvalue);
    }
    return resjson;
}
// Json(ParentId,Skip,Limie,SonNum)
Json::Value MoDB::getFatherComment(Json::Value &queryjson)
{

    int64_t parentid = stoll(queryjson["ParentId"].asString());
    int skip = stoi(queryjson["Skip"].asString());
    int limit = stoi(queryjson["Limit"].asString());
    int sonnum = stoi(queryjson["SonNum"].asString());

    mongocxx::pipeline pipe;
    bsoncxx::builder::stream::document document{};
    // 匹配ParentId
    pipe.match({make_document(kvp("ParentId", parentid))});
    // 跳过多少条
    pipe.skip(skip);
    // 限制多少条
    pipe.limit(limit);
    // 将子评论进行求个数
    document
        << "$set" << open_document
        << "Child_Total" << open_document
        << "$size"
        << "$Child_Comments"
        << close_document << close_document;

    pipe.append_stage(document.view());
    document.clear();
    // 限制子评论的个数
    document
        << "$set" << open_document
        << "Child_Comments" << open_document
        << "$slice" << open_array
        << "$Child_Comments" << 0 << sonnum << close_array
        << close_document << close_document;

    pipe.append_stage(document.view());
    document.clear();
    // 将数组拆散
    document
        << "path"
        << "$Child_Comments"
        << "preserveNullAndEmptyArrays" << true;
    pipe.unwind(document.view());
    document.clear();
    // 将子评论的用户id和用户表进行外连接
    document
        << "from"
        << "User"
        << "localField"
        << "Child_Comments.AuthorId"
        << "foreignField"
        << "_id"
        << "as"
        << "Child_Comments.Author";
    pipe.lookup(document.view());
    document.clear();

    // 将其合并
    document
        << "_id"
        << "$_id"
        << "ParentId" << open_document
        << "$first"
        << "$ParentId" << close_document
        << "Content" << open_document
        << "$first"
        << "$Content" << close_document
        << "Likes" << open_document
        << "$first"
        << "$Likes" << close_document
        << "AuthorId" << open_document
        << "$first"
        << "$AuthorId" << close_document
        << "CreateTime" << open_document
        << "$first"
        << "$CreateTime" << close_document
        << "Child_Total" << open_document
        << "$first"
        << "$Child_Total" << close_document
        << "Child_Comments" << open_document
        << "$push"
        << "$Child_Comments" << close_document;
    pipe.group(document.view());
    document.clear();
    // 将父评论的用户Id和用户表进行外连接
    document
        << "from"
        << "User"
        << "localField"
        << "AuthorId"
        << "foreignField"
        << "_id"
        << "as"
        << "Author";
    pipe.lookup(document.view());
    document.clear();
    // 将不需要的字段消除
    document
        << "$unset" << open_array
        << "AuthorId"
        << "Author.Account"
        << "Author.PassWord"
        << "Author.Likes"
        << "Child_Comments.Author.Account"
        << "Child_Comments.Author.PassWord"
        << "Child_Comments.Author.Likes"
        << close_array;
    pipe.append_stage(document.view());
    document.clear();

    Json::Reader reader;
    Json::Value jsoninfo;
    mongocxx::cursor cursor = commentcoll.aggregate(pipe);
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        jsoninfo.append(jsonvalue);
    }
    Json::Value resjson;
    resjson["Total"] = to_string(commentcoll.count_documents({make_document(kvp("ParentId", parentid))}));
    resjson["Info"] = jsoninfo;
    cout << resjson.toStyledString() << endl;
    return resjson;
}
// Json(ParentId,Skip,Limit)
Json::Value MoDB::getSonComment(Json::Value &queryjson)
{
    int64_t _id = stoll(queryjson["ParentId"].asString());
    int skip = stoi(queryjson["Skip"].asString());
    int limit = stoi(queryjson["Limit"].asString());

    mongocxx::pipeline pipe;
    bsoncxx::builder::stream::document document{};
    // 匹配id
    pipe.match({make_document(kvp("_id", _id))});

    // 将子评论进行求个数
    document
        << "$set" << open_document
        << "Child_Total" << open_document
        << "$size"
        << "$Child_Comments"
        << close_document << close_document;

    pipe.append_stage(document.view());
    document.clear();
    // 限制子评论个数
    document
        << "$set" << open_document
        << "Child_Comments" << open_document
        << "$slice" << open_array
        << "$Child_Comments" << skip << limit << close_array
        << close_document << close_document;

    pipe.append_stage(document.view());
    document.clear();
    // 将数组拆散
    document
        << "path"
        << "$Child_Comments"
        << "preserveNullAndEmptyArrays" << true;
    pipe.unwind(document.view());
    document.clear();
    // 将子评论的用户id和用户表进行外连接
    document
        << "from"
        << "User"
        << "localField"
        << "Child_Comments.AuthorId"
        << "foreignField"
        << "_id"
        << "as"
        << "Child_Comments.Author";
    pipe.lookup(document.view());
    document.clear();

    // 将其合并
    document
        << "_id"
        << "$_id"
        << "Child_Total" << open_document
        << "$first"
        << "$Child_Total" << close_document
        << "Child_Comments" << open_document
        << "$push"
        << "$Child_Comments" << close_document;
    pipe.group(document.view());
    document.clear();

    document
        << "$unset" << open_array
        << "Child_Comments.Author.Account"
        << "Child_Comments.Author.PassWord"
        << "Child_Comments.Author.Likes"
        << close_array;
    pipe.append_stage(document.view());
    document.clear();

    Json::Reader reader;
    Json::Value resjson;
    mongocxx::cursor cursor = commentcoll.aggregate(pipe);
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        resjson.append(jsonvalue);
    }
    return resjson;
}
Json::Value MoDB::getAllArticle()
{
    Json::Reader reader;
    Json::Value resjson;
    mongocxx::cursor cursor = articlecoll.find({});
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        resjson.append(jsonvalue);
    }
    return resjson;
}

// Json(ParentId,Content,AuthorId,AuthorName,AuthorAvatar)
Json::Value MoDB::InsertFatherComment(Json::Value &insertjson)
{
    uuid.init(1, 1);
    int64_t id = uuid.nextid();
    int64_t parentid = atoll(insertjson["ParentId"].asString().data());
    string content = insertjson["Content"].asString();
    int64_t authorid = atoll(insertjson["AuthorId"].asString().data());
    string authorname = insertjson["AuthorName"].asString();
    string authoravatar = insertjson["AuthorAvatar"].asString();
    bsoncxx::builder::stream::document document{};
    document
        << "_id" << id
        << "ParentId" << parentid
        << "Content" << content.data()
        << "Author" << open_document
        << "AuthorId" << authorid
        << "AuthorName" << authorname.data()
        << "AuthorAvatar" << authoravatar.data()
        << close_document
        << "Likes" << 0
        << "CreateTime" << GetTime().data()
        << "Child_Comments" << open_array
        << close_array;

    std::cout << "the document:" << bsoncxx::to_json(document) << std::endl;
    commentcoll.insert_one(document.view());

    Json::Value resjson;
    resjson["_id"] = to_string(id);
    return resjson;
}

// Json(ParentId,Content,AuthorId,AuthorName,AuthorAvatar)
Json::Value MoDB::InsertSonComment(Json::Value &insertjson)
{
    int64_t parentid = stoll(insertjson["ParentId"].asString().data());
    int64_t id = uuid.nextid();
    string content = insertjson["Content"].asString();
    int64_t authorid = stoll(insertjson["AuthorId"].asString().data());
    string authorname = insertjson["AuthorName"].asString();
    string authoravatar = insertjson["AuthorAvatar"].asString();
    bsoncxx::builder::stream::document document{};
    document
        << "$addToSet" << open_document
        << "Child_Comments" << open_document
        << "_id" << id
        << "Content"
        << content.data()
        << "Author" << open_document
        << "AuthorId" << authorid
        << "AuthorName"
        << authorname.data()
        << "AuthorAvatar"
        << authoravatar.data()
        << close_document
        << "Likes" << 0
        << "CreateTime"
        << GetTime().data()
        << close_document
        << close_document;
    std::cout << "the document:" << bsoncxx::to_json(document) << std::endl;

    commentcoll.update_one({make_document(kvp("_id", parentid))}, document.view());

    Json::Value resjson;
    resjson["_id"] = to_string(id);
    return resjson;
}
MoDB::MoDB()
{
}
MoDB::~MoDB()
{
}