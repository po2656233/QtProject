#ifndef CHINESECHESSUI_H
#define CHINESECHESSUI_H

#include "base/UI/gamemap.h"

namespace Ui {
class ChineseChessUI;
}
class QLabel;
class QPropertyAnimation;
class ChineseChessLogic;
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
    //    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //    void contextMenuEvent(QContextMenuEvent *event);
    //    // 鼠标事件——控制显示
    //    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    void init(bool isRedStart);
    QRect adjustRect(const QPoint& pt);//点落至的区域
    QPoint toChessBoardPos(const QRect& rect);//转为 棋盘上的点
    bool killPiece(const QRect& rect);//吃掉棋子
    void resetChessBoard();//重置棋盘
    void turnTime(bool isRed);//时间轮换

private slots:
    void onWinner();


private:
    Ui::ChineseChessUI *ui;
    bool                m_isSelect;
    QPoint              m_toPoint;//移动至
    bool                m_isRed;
    QLabel*             m_selectUI;
    QLabel*             m_curPieces;
    QPropertyAnimation* m_animation;
    ChineseChessLogic*  m_logic;
};

#endif // CHINESECHESSUI_H
