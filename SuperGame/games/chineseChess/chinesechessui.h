////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): chinesechessui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-07-22
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef CHINESECHESSUI_H
#define CHINESECHESSUI_H

#include "base/baseui/gamemap.h"

namespace Ui {
class ChineseChessUI;
}
class QLabel;
class QPropertyAnimation;
class ChineseChessLogic;
class QTimer;
class ChineseChessUI : public GameMap
{
    Q_OBJECT

public:
    explicit ChineseChessUI(QWidget *parent = 0);
    ~ChineseChessUI();


    void enterScene(int scene,const std::string& data); //进入场景
    void changeState(int state,const std::string& data);//状态改变
    bool gameHandle(int code,const std::string& data);//游戏逻辑处理
    bool updateInfo(int code, const std::string& data);//更新游戏信息

    
protected:
    // 拖拽界面
    bool eventFilter(QObject *watched, QEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    void init(bool isRedStart);//初始化
    void resetChessBoard();//重置棋盘(含定时器)

    bool killPiece(const QRect& rect);//吃掉棋子
    void selectPiece(const QRect& rect);//选中棋子

    void turnTime(bool isRed);//时间轮换
    QRect adjustRect(const QPoint& pt);//点落至的区域
    QPoint toChessBoardPos(const QRect& rect);//转为 棋盘上的点


private slots:
    void onWinner();
    void onTimeout();
private:
    Ui::ChineseChessUI *ui;
    bool                m_isRed;
    bool                m_isSelect;
    int                 m_remainTime;//剩余时间
    QPoint              m_toPoint;//移动至
    QRect               m_preSelect;//上一次选中
    QLabel*             m_selectUI;
    QLabel*             m_curPieces;
    QTimer*             m_timer;

    QPropertyAnimation* m_animation;
    ChineseChessLogic*  m_logic;
};

#endif // CHINESECHESSUI_H
