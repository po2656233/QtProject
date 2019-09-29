////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): gameframeui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-15
/// 功能(@brief) ：   平台（场景）
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef GAMEFRAMEUI_H
#define GAMEFRAMEUI_H

#include "base/baseui/uibase.h"
#include <login.pb.h>


namespace Ui {
class GameFrameUI;
}

class QGridLayout;
class GameFrameUI : public UIbase
{
    Q_OBJECT

public:
    explicit GameFrameUI(QWidget *parent = 0);
    ~GameFrameUI();
    void updateUserInfo(const go::UserInfo& userInfo);
    void updateRoomInfo(const go::RoomInfo& roomInfo);
private:
    Ui::GameFrameUI *ui;

};

#endif // GAMEFRAMEUI_H
