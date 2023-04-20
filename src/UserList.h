#ifndef USERLIST_H
#define USERLIST_H
#include <jsoncpp/json/json.h>
#include <unordered_map>

class UserList
{
public:
    // 局部静态特性的方式实现单实例
    static UserList *GetInstance();
    // 注册用户
    Json::Value RegisterUser(Json::Value &registerjson);

    // 登录用户
    Json::Value LoginUser(Json::Value &loginjson);

    // 更新用户题目信息
    bool UpdateUserProblemInfo(Json::Value &updatejson);

    // 获取用户Rank排名
    Json::Value SelectUserRank(Json::Value &queryjson);

    // 获取用户大部分信息
    Json::Value SelectUserInfo(Json::Value &queryjson);

    // 更新用户信息
    Json::Value UpdateUserInfo(Json::Value &updatejson);

    // 获取用户信息以供修改
    Json::Value SelectUserUpdateInfo(Json::Value &queryjson);

    /*
        功能：分页查询用户列表
        传入：Json(Page,PageSize)
        传出：Json(_id,NickName,PersonalProfile,School,Major,JoinTime,Authority)
    */
    Json::Value SelectUserSetInfo(Json::Value &queryjson);

    // 删除用户
    Json::Value DeleteUser(Json::Value &deletejson);

    // 初始化用户权限
    bool InitUserAuthority();

    // 获取用户权限
    int GetUserAuthority(Json::Value &json);

    // 是否是普通用户或以上
    bool IsOrdinaryUser(Json::Value &json);

    // 是否是作者本人
    bool IsAuthor(Json::Value &json);

    // 是否是管理员
    bool IsAdministrator(Json::Value &json);

private:
    UserList();
    ~UserList();

private:
    // 用户权限的哈希表，键：用户ID，值：用户权限
    std::unordered_map<int64_t, int> UserAuthorityMap;
};
#endif