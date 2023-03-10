#ifndef COMMENT_H
#define COMMENT_H
#include <jsoncpp/json/json.h>
#include <string>
// 讨论类
class Comment
{
public:
    // 局部静态特性的方式实现单实例
    static Comment &GetInstance();

    // 获取父评论
    Json::Value getFatherComment(Json::Value &queryjson);

    // 获取子评论
    Json::Value getSonComment(Json::Value &queryjson);

    // 插入父评论
    Json::Value InsertFatherComment(Json::Value &insertjson);

    // 插入子评论
    Json::Value InsertSonComment(Json::Value &insertjson);

private:
    Comment();
    ~Comment();
};
#endif