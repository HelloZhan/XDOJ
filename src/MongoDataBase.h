#ifndef MONGODATABASE_H
#define MONGODATABASE_H
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
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

    // Disscuss 讨论

    // 查询所有讨论
    Json::Value getAllDiscuss();

    Json::Value getAllComment();

    Json::Value getAllArticle();

private:
    // 必须只能一个连接
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::client client{mongocxx::uri{}};
    mongocxx::database db;
    mongocxx::collection articlecoll;

    mongocxx::collection discusscoll;

    mongocxx::collection commentcoll;

    MoDB();
    ~MoDB();
};

#endif