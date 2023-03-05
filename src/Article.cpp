#include "Article.h"

Article &Article::GetInstance()
{
    static Article article;
    return article;
}

Json::Value Article::getAllDiscuss()
{
    return MoDB::GetInstance().getAllArticle();
}

Article::Article()
{
}
Article::~Article()
{
}