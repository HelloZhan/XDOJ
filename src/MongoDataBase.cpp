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
}
/*
    功能：注册用户
    前端传入
    Json(NickName,Account,PassWord,PersonalProfile,School,Major)
    后端传出
    Json(Result,Reason)
*/
Json::Value MoDB::RegisterUser(Json::Value &registerjson)
{
    Json::Value resjson;
    string account = registerjson["Account"].asString();

    auto client = pool.acquire();
    mongocxx::collection usercoll = (*client)["XDOJ"]["User"];

    mongocxx::cursor cursor = usercoll.find({make_document(kvp("Account", account.data()))});
    // 判断账户是否存在
    if (cursor.begin() != cursor.end())
    {
        resjson["Result"] = "Fail";
        resjson["Reason"] = "账户已存在，请重新填写！";
        return resjson;
    }
    string nickname = registerjson["NickName"].asString();
    // 检查昵称是否存在
    cursor = usercoll.find({make_document(kvp("NickName", nickname.data()))});
    if (cursor.begin() != cursor.end())
    {
        resjson["Result"] = "Fail";
        resjson["Reason"] = "昵称已存在，请重新填写！";
        return resjson;
    }
    string password = registerjson["PassWord"].asString();
    string personalprofile = registerjson["PersonalProfile"].asString();
    string school = registerjson["School"].asString();
    string major = registerjson["Major"].asString();
    int64_t id = uuid.nextid();
    string jointime = GetTime();
    // 默认头像
    string avatar = "http://192.168.49.132:8081/image/1";
    // 插入
    bsoncxx::builder::stream::document document{};
    document
        << "_id" << id
        << "Avatar" << avatar.data()
        << "NickName" << nickname.data()
        << "Account" << account.data()
        << "PassWord" << password.data()
        << "PersonalProfile" << personalprofile.data()
        << "School" << school.data()
        << "Major" << major.data()
        << "JoinTime" << jointime.data()
        << "CommentLikes" << open_array << close_array
        << "ACProblems" << open_array << close_array
        << "SubmitNum" << 0
        << "Authority" << 3;

    usercoll.insert_one(document.view());

    resjson["Result"] = "Success";
    resjson["Reason"] = "注册成功！";
    return resjson;
}
/*
    功能：登录用户
    前端传入
    Json(Account,PassWord)
    后端传出
    Json(Result,Reason,Info(_id,NickName,Avatar...))用户的全部信息，详情请见MongoDB集合表
*/
Json::Value MoDB::LoginUser(Json::Value &loginjson)
{
    Json::Value resjson;
    string account = loginjson["Account"].asString();
    string password = loginjson["PassWord"].asString();

    auto client = pool.acquire();
    mongocxx::collection usercoll = (*client)["XDOJ"]["User"];

    mongocxx::pipeline pipe;
    bsoncxx::builder::stream::document document{};
    document
        << "Account" << account.data()
        << "PassWord" << password.data();
    pipe.match(document.view());

    // 匹配账号和密码
    mongocxx::cursor cursor = usercoll.aggregate(pipe);
    if (cursor.begin() == cursor.end())
    {
        resjson["Result"] = "Fail";
        resjson["Reason"] = "账户或密码错误！";
        return resjson;
    }
    // 匹配成功
    Json::Reader reader;
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        resjson["Info"] = jsonvalue;
    }
    resjson["Result"] = "Success";
    resjson["Reason"] = "登录成功！";

    return resjson;
}
/*
    功能：更新用户的题目信息，当用户提交代码时
    传入：Json(UserId,ProblemId,Status)
    传出：bool(如果AC是否向其添加)
*/
bool MoDB::UpdateUserProblemInfo(Json::Value &updatejson)
{
    int64_t userid = stoll(updatejson["UserId"].asString());
    int problemid = stoi(updatejson["ProblemId"].asString());
    int status = stoi(updatejson["Status"].asString());

    // 将用户提交数目加一
    auto client = pool.acquire();
    mongocxx::collection usercoll = (*client)["XDOJ"]["User"];

    bsoncxx::builder::stream::document document{};
    document
        << "$inc" << open_document
        << "SubmitNum" << 1 << close_document;

    usercoll.update_one({make_document(kvp("_id", userid))}, document.view());

    // 如果AC了
    if (status == 2)
    {
        // 查询AC题目是否已经添加至ACProblems的数组中
        mongocxx::pipeline pipe;
        pipe.match({make_document(kvp("_id", userid))});
        document.clear();
        document
            << "IsHasAc" << open_document
            << "$in" << open_array << problemid << "$ACProblems" << close_array
            << close_document;
        pipe.project(document.view());
        Json::Reader reader;
        Json::Value tmpjson;
        mongocxx::cursor cursor = usercoll.aggregate(pipe);

        for (auto doc : cursor)
        {
            reader.parse(bsoncxx::to_json(doc), tmpjson);
        }
        // 未添加
        if (tmpjson["IsHasAc"].asBool() == false)
        {
            document.clear();
            document
                << "$push" << open_document
                << "ACProblems" << problemid
                << close_document;
            usercoll.update_one({make_document(kvp("_id", userid))}, document.view());
            return true;
        }
        return false;
    }
    return false;
}

/*
    功能：获取用户的Rank排名
    传入：Json(Page,PageSize)
    传出：Json(_id,Rank,Avatar,NickName,PersonalProfile,SubmitNum,ACNum),TotalNum
*/
Json::Value MoDB::SelectUserRank(Json::Value &queryjson)
{
    int page = stoi(queryjson["Page"].asString());
    int pagesize = stoi(queryjson["PageSize"].asString());
    int skip = (page - 1) * pagesize;

    auto client = pool.acquire();
    mongocxx::collection usercoll = (*client)["XDOJ"]["User"];
    bsoncxx::builder::stream::document document{};
    mongocxx::pipeline pipe, pipetot;
    Json::Reader reader;
    Json::Value resjson;

    // 获取总条数
    pipetot.count("TotalNum");
    mongocxx::cursor cursor = usercoll.aggregate(pipetot);
    for (auto doc : cursor)
    {
        reader.parse(bsoncxx::to_json(doc), resjson);
    }

    document
        << "$set" << open_document
        << "ACNum" << open_document
        << "$size"
        << "$ACProblems"
        << close_document << close_document;
    pipe.append_stage(document.view());
    pipe.sort({make_document(kvp("ACNum", -1))});
    pipe.skip(skip);
    pipe.limit(pagesize);
    document.clear();
    document
        << "Avatar" << 1
        << "NickName" << 1
        << "PersonalProfile" << 1
        << "SubmitNum" << 1
        << "ACNum" << 1;
    pipe.project(document.view());

    cursor = usercoll.aggregate(pipe);
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        resjson["ArrayInfo"].append(jsonvalue);
    }
    // 添加Rank排名
    int currank = (page - 1) * pagesize + 1;
    for (int i = 0; i < resjson["ArrayInfo"].size(); i++)
    {
        resjson["ArrayInfo"][i]["Rank"] = currank++;
    }
    return resjson;
}

/*
    功能：获取全部题目信息（用于ProblemSet类进行初始化）
    Json(_id,Title,Description,JudgeNum)
*/
Json::Value MoDB::getAllProblem()
{
    Json::Value resjson;
    Json::Reader reader;
    auto client = pool.acquire();
    mongocxx::collection problemcoll = (*client)["XDOJ"]["Problem"];
    mongocxx::cursor cursor = problemcoll.find({});
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        resjson.append(jsonvalue);
    }
    return resjson;
}
/*
    功能：分页获取题目列表（包含查询条件，暂时未添加）
    前端传入
    Json(QueryType,Page,PageSize)
    后端传出
    Json(([ProblemId,Title,SubmitNum,CENum,ACNum,WANum,TLENum,MLENum,SENum]),TotalNum)
*/
Json::Value MoDB::getProblemSet(Json::Value &queryjson)
{
    string querytype = queryjson["QueryType"].asString();
    int page = stoi(queryjson["Page"].asString());
    int pagesize = stoi(queryjson["PageSize"].asString());
    int skip = (page - 1) * pagesize;

    auto client = pool.acquire();
    mongocxx::collection problemcoll = (*client)["XDOJ"]["Problem"];
    Json::Value resjson;
    Json::Reader reader;
    mongocxx::pipeline pipe, pipetot;
    bsoncxx::builder::stream::document document{};

    // 获取总条数
    pipetot.count("TotalNum");
    mongocxx::cursor cursor = problemcoll.aggregate(pipetot);
    for (auto doc : cursor)
    {
        reader.parse(bsoncxx::to_json(doc), resjson);
    }
    // TODO:查询条件

    // 排序
    pipe.sort({make_document(kvp("_id", 1))});
    // 跳过
    pipe.skip(skip);
    // 限制
    pipe.limit(pagesize);
    // 进行
    document
        << "ProblemId"
        << "$_id"
        << "Title" << 1
        << "SubmitNum" << 1
        << "CENum" << 1
        << "ACNum" << 1
        << "WANum" << 1
        << "TLENum" << 1
        << "MLENum" << 1;
    pipe.project(document.view());

    Json::Value arryjson;
    cursor = problemcoll.aggregate(pipe);
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        arryjson.append(jsonvalue);
    }
    resjson["Array"] = arryjson;
    return resjson;
}
/*
    功能：更新题目的状态数量
    传入：Json(ProblemId,Status)
    传出：bool
*/
bool MoDB::UpdateProblemStatusNum(Json::Value &updatejson)
{
    int64_t problemid = stoll(updatejson["ProblemId"].asString());
    int status = stoi(updatejson["Status"].asString());

    string statusnum = "";
    if (status == 1)
        statusnum = "CENum";
    else if (status == 2)
        statusnum = "ACNum";
    else if (status == 3)
        statusnum = "WANum";
    else if (status == 4)
        statusnum = "RENum";
    else if (status == 5)
        statusnum = "TLENum";
    else if (status == 6)
        statusnum = "MLENum";
    else if (status == 7)
        statusnum = "SENum";

    auto client = pool.acquire();
    mongocxx::collection problemcoll = (*client)["XDOJ"]["Problem"];
    bsoncxx::builder::stream::document document{};
    document
        << "$inc" << open_document
        << "SubmitNum" << 1
        << statusnum << 1 << close_document;

    problemcoll.update_one({make_document(kvp("_id", problemid))}, document.view());
    return true;
}
/*
    功能：向测评表插入一条待测评记录
    传入：Json(ProblemId,UserId,UserNickName,ProblemTitle,Language,Code)
    传出：SubmitId测评的ID
*/
string MoDB::InsertStatusRecord(Json::Value &insertjson)
{
    int64_t id = uuid.nextid();
    int64_t problemid = stoll(insertjson["ProblemId"].asString());
    int64_t userid = stoll(insertjson["UserId"].asString());
    string usernickname = insertjson["UserNickName"].asString();
    string problemtitle = insertjson["ProblemTitle"].asString();
    string language = insertjson["Language"].asString();
    string code = insertjson["Code"].asString();

    auto client = pool.acquire();
    mongocxx::collection statusrecordcoll = (*client)["XDOJ"]["StatusRecord"];
    bsoncxx::builder::stream::document document{};

    document
        << "_id" << id
        << "ProblemId" << problemid
        << "UserId" << userid
        << "UserNickName" << usernickname.data()
        << "ProblemTitle" << problemtitle.data()
        << "Status" << 0
        << "RunTime"
        << "0MS"
        << "RunMemory"
        << "0MB"
        << "Length"
        << "0B"
        << "Language" << language.data()
        << "SubmitTime" << GetTime().data()
        << "Code" << code.data()
        << "ComplierInfo"
        << ""
        << "TestInfo" << open_array << close_array;
    statusrecordcoll.insert_one(document.view());
    return to_string(id);
}

/*
    功能：更新测评记录并返回测评记录
    传入：Json(SubmitId,Status,RunTime,RunMemory,Length,ComplierInfo,
    TestInfo[(Status,StandardOutput,PersonalOutput,RunTime,RunMemory)])

    传出：Json() 测评记录
*/
Json::Value MoDB::UpdateStatusRecord(Json::Value &updatejson)
{
    int64_t submitid = stoll(updatejson["SubmitId"].asString());
    int status = stoi(updatejson["Status"].asString());
    string runtime = updatejson["RunTime"].asString();
    string runmemory = updatejson["RunMemory"].asString();
    string length = updatejson["Length"].asString();
    string complierinfo = updatejson["ComplierInfo"].asString();

    // 更新测评记录
    auto client = pool.acquire();
    mongocxx::collection statusrecordcoll = (*client)["XDOJ"]["StatusRecord"];
    bsoncxx::builder::stream::document document{};
    auto in_array = document
                    << "$set" << open_document
                    << "Status" << status
                    << "RunTime" << runtime.data()
                    << "RunMemory" << runmemory.data()
                    << "Length" << length.data()
                    << "ComplierInfo" << complierinfo.data()
                    << "TestInfo" << open_array;

    for (int i = 0; i < updatejson["TestInfo"].size(); i++)
    {
        int teststatus = stoi(updatejson["TestInfo"][i]["Status"].asString());
        string standardoutput = updatejson["TestInfo"][i]["StandardOutput"].asString();
        string personaloutput = updatejson["TestInfo"][i]["PersonalOutput"].asString();
        string testruntime = updatejson["TestInfo"][i]["RunTime"].asString();
        string testrunmemory = updatejson["TestInfo"][i]["RunMemory"].asString();
        in_array = in_array << open_document
                            << "Status" << teststatus
                            << "StandardOutput" << standardoutput
                            << "PersonalOutput" << personaloutput
                            << "RunTime" << testruntime
                            << "RunMemory" << testrunmemory << close_document;
    }
    bsoncxx::document::value doc = in_array << close_array << close_document << finalize;

    statusrecordcoll.update_one({make_document(kvp("_id", submitid))}, doc.view());
    // 查询测评记录
    Json::Reader reader;
    Json::Value resjson;
    mongocxx::cursor cursor = statusrecordcoll.find({{make_document(kvp("_id", submitid))}});
    for (auto doc : cursor)
    {
        reader.parse(bsoncxx::to_json(doc), resjson);
    }
    cout << resjson.toStyledString() << endl;
    return resjson;
}
/*
    功能：分页查询测评记录
    传入：Json(QueryType,PageSize,Page)
    传出：测评全部信息，详情请见MongoDB集合表
*/
Json::Value MoDB::SelectStatusRecord(Json::Value &queryjson)
{
    string querytype = queryjson["QueryType"].asString();
    int page = stoi(queryjson["Page"].asString());
    int pagesize = stoi(queryjson["PageSize"].asString());
    int skip = (page - 1) * pagesize;
    Json::Value resjson;
    Json::Reader reader;
    mongocxx::pipeline pipe, pipetot;
    bsoncxx::builder::stream::document document{};
    auto client = pool.acquire();
    mongocxx::collection statusrecordcoll = (*client)["XDOJ"]["StatusRecord"];

    // 获取总条数
    pipetot.count("TotalNum");
    mongocxx::cursor cursor = statusrecordcoll.aggregate(pipetot);
    for (auto doc : cursor)
    {
        reader.parse(bsoncxx::to_json(doc), resjson);
    }
    // TODO:查询条件

    // 排序
    pipe.sort({make_document(kvp("SubmitTime", -1))});
    // 跳过
    pipe.skip(skip);
    // 限制
    pipe.limit(pagesize);

    Json::Value arryjson;
    cursor = statusrecordcoll.aggregate(pipe);
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        arryjson.append(jsonvalue);
    }
    resjson["Array"] = arryjson;
    return resjson;
}

/*
    功能：添加讨论
    传入：Json(Title,Content,ParentId,UserId) 如果是父讨论ParentId=0
    传出：bool
*/
bool MoDB::InsertDiscuss(Json::Value &insertjson)
{
    uuid.init(1, 1);
    auto id = uuid.nextid();
    string title = insertjson["Title"].asString();
    string content = insertjson["Content"].asString();
    int64_t parentid = atoll(insertjson["ParentId"].asString().data());
    int64_t userid = atoll(insertjson["UserId"].asString().data());

    auto client = pool.acquire();
    mongocxx::collection discusscoll = (*client)["XDOJ"]["Discuss"];
    bsoncxx::builder::stream::document document{};
    document
        << "_id" << id
        << "Title" << title.data()
        << "Content" << content.data()
        << "ParentId" << parentid
        << "UserId" << userid
        << "Views" << 0
        << "Comments" << 0
        << "CreateTime" << GetTime().data()
        << "UpdateTime" << GetTime().data();

    discusscoll.insert_one(document.view());
}
/*
    功能：分页查询讨论
    传入：Json(ParentId,Page,PageSize)
    传出：Json(_id,Title,Views,Comments,CreateTime,User.Avatar,User.NickName)
*/
Json::Value MoDB::SelectDiscuss(Json::Value &queryjson)
{
    int64_t parentid = stoll(queryjson["ParentId"].asString());
    int page = stoi(queryjson["Page"].asString());
    int pagesize = stoi(queryjson["PageSize"].asString());
    int skip = (page - 1) * pagesize;

    Json::Value resjson;
    Json::Reader reader;
    bsoncxx::builder::stream::document document{};
    mongocxx::pipeline pipe, pipetot;

    auto client = pool.acquire();
    mongocxx::collection discusscoll = (*client)["XDOJ"]["Discuss"];
    // 获取总条数
    pipetot.match({make_document(kvp("ParentId", parentid))});
    pipetot.count("TotalNum");
    mongocxx::cursor cursor = discusscoll.aggregate(pipetot);
    for (auto doc : cursor)
    {
        reader.parse(bsoncxx::to_json(doc), resjson);
    }

    pipe.match({make_document(kvp("ParentId", parentid))});
    pipe.sort({make_document(kvp("CreateTime", -1))});
    pipe.skip(skip);
    pipe.limit(pagesize);
    document
        << "from"
        << "User"
        << "localField"
        << "UserId"
        << "foreignField"
        << "_id"
        << "as"
        << "User";
    pipe.lookup(document.view());

    document.clear();
    document
        << "Title" << 1
        << "Views" << 1
        << "Comments" << 1
        << "CreateTime" << 1
        << "User.Avatar" << 1
        << "User.NickName" << 1;
    pipe.project(document.view());

    cursor = discusscoll.aggregate(pipe);
    for (auto doc : cursor)
    {
        Json::Value jsonvalue;
        reader.parse(bsoncxx::to_json(doc), jsonvalue);
        resjson["ArrayInfo"].append(jsonvalue);
    }
    return resjson;
}

/*
    功能：查询讨论的详细内容，并且将其浏览量加一
    传入：Json(DiscussId)
    传出：Json(Content)
*/
Json::Value MoDB::SelectDiscussContent(Json::Value &queryjson)
{
    int64_t discussid = stoll(queryjson["DiscussId"].asString());

    auto client = pool.acquire();
    mongocxx::collection discusscoll = (*client)["XDOJ"]["Discuss"];
    // 浏览量加一
    bsoncxx::builder::stream::document document{};
    document
        << "$inc" << open_document
        << "Views" << 1 << close_document;
    discusscoll.update_one({make_document(kvp("_id", discussid))}, document.view());

    // 查询Content
    mongocxx::pipeline pipe;
    pipe.match({make_document(kvp("_id", 603757867216015360))});
    document.clear();
    document
        << "Content" << 1;
    pipe.project(document.view());
    mongocxx::cursor cursor = discusscoll.aggregate(pipe);

    Json::Reader reader;
    Json::Value resjson;
    for (auto doc : cursor)
    {
        reader.parse(bsoncxx::to_json(doc), resjson);
    }

    return resjson;
}
Json::Value MoDB::getAllDiscuss()
{
    auto client = pool.acquire();
    mongocxx::collection discusscoll = (*client)["XDOJ"]["Discuss"];
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

Json::Value MoDB::getAllArticle()
{
    auto client = pool.acquire();
    mongocxx::collection articlecoll = (*client)["XDOJ"]["Article"];
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

// Json(ParentId,Skip,Limie,SonNum)
Json::Value MoDB::getFatherComment(Json::Value &queryjson)
{

    int64_t parentid = stoll(queryjson["ParentId"].asString());
    int skip = stoi(queryjson["Skip"].asString());
    int limit = stoi(queryjson["Limit"].asString());
    int sonnum = stoi(queryjson["SonNum"].asString());
    auto client = pool.acquire();
    mongocxx::collection commentcoll = (*client)["XDOJ"]["Comment"];
    mongocxx::pipeline pipe;
    bsoncxx::builder::stream::document document{};
    // 匹配ParentId
    pipe.match({make_document(kvp("ParentId", parentid))});
    // 按照时间先后顺序
    pipe.sort({make_document(kvp("CreateTime", 1))});
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
    // 按照时间先后顺序
    pipe.sort({make_document(kvp("CreateTime", 1))});

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

    auto client = pool.acquire();
    mongocxx::collection commentcoll = (*client)["XDOJ"]["Comment"];
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
        reader.parse(bsoncxx::to_json(doc), resjson);
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
    string createtime = GetTime();

    auto client = pool.acquire();
    mongocxx::collection commentcoll = (*client)["XDOJ"]["Comment"];
    bsoncxx::builder::stream::document document{};
    document
        << "_id" << id
        << "ParentId" << parentid
        << "Content" << content.data()
        << "AuthorId" << authorid
        << "Likes" << 0
        << "CreateTime" << createtime.data()
        << "Child_Comments" << open_array
        << close_array;

    std::cout << "the document:" << bsoncxx::to_json(document) << std::endl;
    commentcoll.insert_one(document.view());

    Json::Value resjson;
    resjson["_id"] = to_string(id);
    resjson["CreateTime"] = createtime.data();
    return resjson;
}

// Json(ParentId,Content,AuthorId,AuthorName,AuthorAvatar)
Json::Value MoDB::InsertSonComment(Json::Value &insertjson)
{
    int64_t parentid = stoll(insertjson["ParentId"].asString().data());
    int64_t id = uuid.nextid();
    string content = insertjson["Content"].asString();
    int64_t authorid = stoll(insertjson["AuthorId"].asString().data());
    string createtime = GetTime();

    auto client = pool.acquire();
    mongocxx::collection commentcoll = (*client)["XDOJ"]["Comment"];

    bsoncxx::builder::stream::document document{};
    document
        << "$addToSet" << open_document
        << "Child_Comments" << open_document
        << "_id" << id
        << "Content"
        << content.data()
        << "AuthorId" << authorid
        << "Likes" << 0
        << "CreateTime"
        << createtime.data()
        << close_document
        << close_document;
    std::cout << "the document:" << bsoncxx::to_json(document) << std::endl;

    commentcoll.update_one({make_document(kvp("_id", parentid))}, document.view());

    Json::Value resjson;
    resjson["_id"] = to_string(id);
    resjson["CreateTime"] = createtime;
    return resjson;
}
MoDB::MoDB()
{
}
MoDB::~MoDB()
{
}