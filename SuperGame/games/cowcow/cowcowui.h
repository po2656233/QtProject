////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): cowcowui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-07-22
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef COWCOWUI_H
#define COWCOWUI_H

#include "base/baseui/gamemap.h"
#include "cowcowlogic.h"
namespace Ui {
class CowcowUI;
}

class Poker;
class QTimer;
class QLabel;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;
class QStandardItemModel;

class CowcowLogic;
class UIBetArea;
class CowcowUI : public GameMap
{
    Q_OBJECT

public:
    explicit CowcowUI(QWidget *parent = 0);
    ~CowcowUI();
    /// 父类接口
    void enterScene(int scene,const std::string& data); //进入场景
    void changeState(int state,const std::string& data);//状态改变
    bool gameHandle(int code,const std::string& data);  //逻辑处理
    bool updateInfo(int code, const std::string& data); //更新信息


    /// 槽函数
public slots:
    void onTimeout();// 定时器
    void onPlayBetting(CowcowLogic::Area area, qint64 money);//下注

    /// 事件函数
protected:
    //事件过滤 下注筹码可控制
    bool eventFilter(QObject *watched, QEvent *event);


    ///协议处理接口
private:
    //状态解析
    bool doStateStart(const std::string& data);
    bool doStatePlaying(const std::string& data);
    bool doStateOver(const std::string& data);

    //逻辑处理
    bool doFrameStart(const std::string& data);
    bool doFramePlaying(const std::string& data);
    bool doFrameBetResult(const std::string& data);
    bool doFrameOver(const std::string& data);
    bool doFrameCheckout(const std::string& data);
    bool doFrameResult(const std::string& data);

    //更新处理(广播) UpdatePlayerList
    bool doUpdatePlayerList(const std::string& data);

    ///内部接口
private:
    //初始化
    void init();

    void initCard();

    //区域信息
    UIBetArea *getArea(const CowcowLogic::Area& area);

    // 中奖动画
    void awardAnimation(const std::string& strAward);

    // 发牌动画
    void dealCardAnimation(const std::string& strPlayerCard,const std::string& strBankerCard);

    //允许下注
    void allowPlaying(bool isAllow);

    //飞筹码
    bool addChip(const CowcowLogic::Area& area, int money);

    //清空筹码
    void clearChips();
private:
    Ui::CowcowUI *ui;
    int m_freeTime;
    int m_betTime;
    int m_openTime;
    int m_remainTime;                 // 剩余时长
    int m_curState;                 // 当前状态

    qint64 m_BetGolds;              // 下注金额
    qint64 m_PlayerGold;

    bool m_animatIsFinish;
    CowcowLogic*  m_logic;

    QTimer* m_timer;
    QLabel* m_select;
    QStandardItemModel* m_modelPlayers;
    QParallelAnimationGroup* m_animationGroup;
    QSequentialAnimationGroup* m_cardGroup;
};

#endif // COWCOWUI_H
