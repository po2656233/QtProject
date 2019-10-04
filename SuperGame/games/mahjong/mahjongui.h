////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): mahjongui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-07-22
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef MAHJONGUI_H
#define MAHJONGUI_H

#include "base/baseui/gamemap.h"

namespace Ui {
class Mahjong;
}

class MahjongUI : public GameMap
{
    Q_OBJECT

public:
    explicit MahjongUI(QWidget *parent = 0);
    ~MahjongUI();

    void changeState(int state,const std::string& data);//状态切换
    void enterScene(int scene,const std::string& data);//进入场景
    bool gameHandle(int code,const std::string& data);//游戏处理
    bool updateInfo(int code, const std::string& data);//更新游戏信息

private:
    Ui::Mahjong *ui;
};

#endif // MAHJONG_H
