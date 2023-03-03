#include "MongoDataBase.h"
#include <cstdint>
#include <iostream>

MoDB &MoDB::GetInstance()
{
    static MoDB modb;
    return modb;
}

bool MoDB::InitDB()
{
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::client client{mongocxx::uri{}};

    // 连接数据库
    db = client["XDOJ"];
    // 连接集合
    articlecoll = db["Article"];

    // // 查询 "title:A+B"
    // mongocxx::cursor cursor = articlecoll.find({make_document(kvp("title", "A+B"))});

    // // mongocxx::cursor cursor = coll.find({document{} << "title"
    // //                                                 << "A+B" << finalize});
    // for (auto doc : cursor)
    // {
    //     std::cout << bsoncxx::to_json(doc) << "\n";
    // }
}
MoDB::MoDB()
{
}
MoDB::~MoDB()
{
}