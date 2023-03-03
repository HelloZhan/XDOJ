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

    bool InitDB();

private:
    mongocxx::database db;

    mongocxx::collection articlecoll;

    MoDB();
    ~MoDB();
};

#endif