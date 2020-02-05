////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): homeui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-15
/// 功能(@brief) ：   主房间（场景）
/// 目的(@target)：
/// 备注(@attention):也称游戏大厅
////////////////////////////////////////////////////

#ifndef HOMEUI_H
#define HOMEUI_H

#include "base/baseui/uibase.h"
#include <login.pb.h>
#include <QMap>
namespace Ui {
class HomeUI;
}

class QGridLayout;
class QSequentialAnimationGroup;
class QParallelAnimationGroup;
class HomeUI : public UIbase
{
    Q_OBJECT

public:
    explicit HomeUI(QWidget *parent = 0);
    ~HomeUI();
    static int S_GameID;  // 当前的游戏ID
    static int S_GameKindID;
    static int S_GameLevel;


    void updateGameList(go::GameList& list);//更新游戏列表
    void clearList();
    void clearAnimations();
signals:
    void enterSig(int,QString);
private slots:
    void on_pushButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::HomeUI *ui;
    bool m_animatIsFinish;
    QGridLayout* m_layoutGames;
    QSequentialAnimationGroup *m_AnimationGoup;
    //QParallelAnimationGroup *m_AnimationGoup;
};

#endif // HOMEUI_H
