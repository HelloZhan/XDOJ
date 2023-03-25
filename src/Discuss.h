#ifndef DISCUSS_H
#define DISCUSS_H
#include <jsoncpp/json/json.h>
#include <string>
// 讨论类
class Discuss
{
public:
    // 局部静态特性的方式实现单实例
    static Discuss &GetInstance();

    // 添加讨论
    Json::Value InsertDiscuss(Json::Value &insertjson);

    // 分页查询讨论
    Json::Value SelectDiscuss(Json::Value &queryjson);

    // 管理员分页查询
    Json::Value SelectDiscussByAdmin(Json::Value &queryjson);

    // 查询
    Json::Value SelectDiscussByEdit(Json::Value &queryjson);

    // 查询讨论的内容
    Json::Value SelectDiscussContent(Json::Value &queryjson);

    // 修改评论数的数量
    bool UpdateDiscussComments(Json::Value &updatejson);

    // 更新讨论
    Json::Value UpdateDiscuss(Json::Value &updatejson);

    // 删除讨论
    Json::Value DeleteDiscuss(Json::Value &deletejson);

private:
    Discuss();
    ~Discuss();
};
#endif