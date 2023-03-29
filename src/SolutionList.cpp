#include "SolutionList.h"
#include "MongoDataBase.h"
// 局部静态特性的方式实现单实例
SolutionList &SolutionList::GetInstance()
{
    static SolutionList solutionlist;
    return solutionlist;
}

Json::Value SolutionList::InsertSolution(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertSolution(insertjson);
}

Json::Value SolutionList::SelectSolutionList(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectSolutionList(queryjson);
}

Json::Value SolutionList::SelectSolutionListByAdmin(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectSolutionListByAdmin(queryjson);
}

Json::Value SolutionList::SelectSolutionByEdit(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectSolutionByEdit(queryjson);
}

Json::Value SolutionList::SelectSolution(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectSolution(queryjson);
}

bool SolutionList::UpdateSolutionComments(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateSolutionComments(updatejson);
}

Json::Value SolutionList::UpdateSolution(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateSolution(updatejson);
}

Json::Value SolutionList::DeleteSolution(Json::Value &deletejson)
{
    return MoDB::GetInstance().DeleteSolution(deletejson);
}

SolutionList::SolutionList()
{
}
SolutionList::~SolutionList()
{
}