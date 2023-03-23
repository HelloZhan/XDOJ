#include "Solution.h"
#include "MongoDataBase.h"
// 局部静态特性的方式实现单实例
Solution &Solution::GetInstance()
{
    static Solution solution;
    return solution;
}

Json::Value Solution::InsertSolution(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertSolution(insertjson);
}

Json::Value Solution::SelectSolution(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectSolution(queryjson);
}

Json::Value Solution::SelectSolutionByEdit(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectSolutionByEdit(queryjson);
}

Json::Value Solution::SelectSolutionContent(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscussContent(queryjson);
}

bool Solution::UpdateSolutionComments(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateSolutionComments(updatejson);
}

Json::Value Solution::UpdateSolution(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateSolution(updatejson);
}

Json::Value Solution::DeleteSolution(Json::Value &deletejson)
{
    return MoDB::GetInstance().DeleteSolution(deletejson);
}

Solution::Solution()
{
}
Solution::~Solution()
{
}