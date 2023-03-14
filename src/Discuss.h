#ifndef DISCUSS_H
#define DISCUSS_H
#include <jsoncpp/json/json.h>
#include <string>
// 讨论类
class Disscuss
{
public:
    // 局部静态特性的方式实现单实例
    static Disscuss &GetInstance();

    // 获取所有讨论
    Json::Value getAllDiscuss();

    // 添加讨论
    bool InsertDiscuss(Json::Value &insertjson);

    // 分页查询讨论
    Json::Value SelectDiscuss(Json::Value &queryjson);

    // 查询讨论的内容
    Json::Value SelectDiscussContent(Json::Value &queryjson);

private:
    Disscuss();
    ~Disscuss();
};
#endif