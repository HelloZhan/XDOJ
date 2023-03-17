#include "Comment.h"
#include "MongoDataBase.h"

Comment &Comment::GetInstance()
{
    static Comment comment;
    return comment;
}
Json::Value Comment::getFatherComment(Json::Value &queryjson)
{
    return MoDB::GetInstance().getFatherComment(queryjson);
}
Json::Value Comment::getSonComment(Json::Value &queryjson)
{
    return MoDB::GetInstance().getSonComment(queryjson);
}

// 插入父评论
Json::Value Comment::InsertFatherComment(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertFatherComment(insertjson);
}

// 插入子评论
Json::Value Comment::InsertSonComment(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertSonComment(insertjson);
}

bool Comment::DeleteArticleComment(Json::Value &deletejson)
{
    return MoDB::GetInstance().DeleteArticleComment(deletejson);
}

Json::Value Comment::DeleteFatherComment(Json::Value &deletejson)
{
    return MoDB::GetInstance().DeleteFatherComment(deletejson);
}

Json::Value Comment::DeleteSonComment(Json::Value &deletejson)
{
    return MoDB::GetInstance().DeleteSonComment(deletejson);
}

Comment::Comment()
{
}
Comment::~Comment()
{
}