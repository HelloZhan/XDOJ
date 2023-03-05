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

    Json::Value getAllComment();

private:
    Comment();
    ~Comment();
};
#endif