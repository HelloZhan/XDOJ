#ifndef ARTICLE_H
#define ARTICLE_H
#include <jsoncpp/json/json.h>
#include <string>
// 讨论类
class Article
{
public:
    // 局部静态特性的方式实现单实例
    static Article &GetInstance();

    Json::Value getAllArticle();

private:
    Article();
    ~Article();
};
#endif