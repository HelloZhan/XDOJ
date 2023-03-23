#include "Announcement.h"
#include "MongoDataBase.h"
// 局部静态特性的方式实现单实例
Announcement &Announcement::GetInstance()
{
    static Announcement solution;
    return solution;
}

Json::Value Announcement::InsertAnnouncement(Json::Value &insertjson)
{
    return MoDB::GetInstance().InsertAnnouncement(insertjson);
}

Json::Value Announcement::SelectAnnouncement(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectAnnouncement(queryjson);
}

Json::Value Announcement::SelectAnnouncementByEdit(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectAnnouncementByEdit(queryjson);
}

Json::Value Announcement::SelectAnnouncementContent(Json::Value &queryjson)
{
    return MoDB::GetInstance().SelectDiscussContent(queryjson);
}

bool Announcement::UpdateAnnouncementComments(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateAnnouncementComments(updatejson);
}

Json::Value Announcement::UpdateAnnouncement(Json::Value &updatejson)
{
    return MoDB::GetInstance().UpdateAnnouncement(updatejson);
}

Json::Value Announcement::DeleteAnnouncement(Json::Value &deletejson)
{
    return MoDB::GetInstance().DeleteAnnouncement(deletejson);
}

Announcement::Announcement()
{
}
Announcement::~Announcement()
{
}