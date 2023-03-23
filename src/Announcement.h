#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H
#include <jsoncpp/json/json.h>
#include <string>
// 公告类
class Announcement
{
public:
    // 局部静态特性的方式实现单实例
    static Announcement &GetInstance();

    // 添加公告
    Json::Value InsertAnnouncement(Json::Value &insertjson);

    // 分页查询公告
    Json::Value SelectAnnouncement(Json::Value &queryjson);

    // 查询
    Json::Value SelectAnnouncementByEdit(Json::Value &queryjson);

    // 查询公告的内容
    Json::Value SelectAnnouncementContent(Json::Value &queryjson);

    // 修改评论数的数量
    bool UpdateAnnouncementComments(Json::Value &updatejson);

    // 更新公告
    Json::Value UpdateAnnouncement(Json::Value &updatejson);

    // 删除公告
    Json::Value DeleteAnnouncement(Json::Value &deletejson);

private:
    Announcement();
    ~Announcement();
};
#endif