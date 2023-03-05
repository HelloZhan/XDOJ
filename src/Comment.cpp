#include "Comment.h"

Comment &Comment::GetInstance()
{
    static Comment comment;
    return comment;
}
Json::Value Comment::getAllDiscuss()
{
    return MoDB::GetInstance().getAllComment();
}
Comment::Comment()
{
}
Comment::~Comment()
{
}