#ifndef SOLUTION_H
#define SOLUTION_H
#include <jsoncpp/json/json.h>
#include <string>
// 讨论类
class Solution
{
public:
    // 局部静态特性的方式实现单实例
    static Solution &GetInstance();

    // 添加讨论
    Json::Value InsertSolution(Json::Value &insertjson);

    // 分页查询讨论
    Json::Value SelectSolution(Json::Value &queryjson);

    // 查询
    Json::Value SelectSolutionByEdit(Json::Value &queryjson);

    // 查询讨论的内容
    Json::Value SelectSolutionContent(Json::Value &queryjson);

    // 修改评论数的数量
    bool UpdateSolutionComments(Json::Value &updatejson);

    // 更新讨论
    Json::Value UpdateSolution(Json::Value &updatejson);

    // 删除讨论
    Json::Value DeleteSolution(Json::Value &deletejson);

private:
    Solution();
    ~Solution();
};
#endif