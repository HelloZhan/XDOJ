#include "MongoDataBase.h"
#include <cstdint>
#include <iostream>
using namespace std;
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

MoDB::MoDB()
{
}
MoDB::~MoDB()
{
}