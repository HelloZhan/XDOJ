#include "Comment.h"
#include "MongoDataBase.h"

Comment &Comment::GetInstance()
{
    static Comment comment;
    return comment;
}
Json::Value Comment::getAllCommentById(std::string &id)
{
    return MoDB::GetInstance().getAllCommentById(id);
}
Comment::Comment()
{
}
Comment::~Comment()
{
}