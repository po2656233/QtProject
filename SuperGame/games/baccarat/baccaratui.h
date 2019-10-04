///////////////////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): baccaratui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-07-22
/// 功能(@brief) ：   百家乐
/// 目的(@target)：   以【开始】-->【下注】-->【结算】为一局。
///         【开始】阶段:玩家准备博弈的一个阶段。
///         【下注】阶段:允许玩家以筹码额，进行区域下注。
///         【结算】阶段:根据开牌结果,结合区域赔率为本轮下注玩家进行结算。
///             结算公式： G = B*L-T
///个人所得(G)=中奖区域下注额(B)*中奖区域赔率(L)-各个区域总下注额之和(T)
/// 备注(@attention):
///         希望广大玩家以历史记录作为每轮博弈的准备，理性博弈。
///////////////////////////////////////////////////////////////////
#ifndef BACCARATUI_H
#define BACCARATUI_H

#include "base/baseui/gamemap.h"

namespace Ui {
class BaccaratUI;
}

class Poker;
class QTimer;
class QLabel;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;
class QStandardItemModel;

class BaccaratLogic;
class UIBetArea;
class BaccaratUI : public GameMap
{
    Q_OBJECT

public:
    explicit BaccaratUI(QWidget *parent = 0);
    ~BaccaratUI();

    /// 父类接口
    void enterScene(int scene,const std::string& data); //进入场景
    void changeState(int state,const std::string& data);//状态改变
    bool gameHandle(int code,const std::string& data);  //逻辑处理
    bool updateInfo(int code, const std::string& data); //更新信息


    /// 槽函数
public slots:
    void onTimeout();// 定时器
    void onPlayBetting(int area, qint64 money);//下注

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
    bool doFrameSetHost(const std::string& data);
    bool doFrameHost(const std::string& data);
    bool doFrameSuperHost(const std::string& data);
    bool doFrameResult(const std::string& data);

    //更新处理(广播) UpdatePlayerList
    bool doUpdatePlayerList(const std::string& data);

    ///内部接口
private:
    //初始化
    void init();

    //区域信息
    UIBetArea *getArea(int area);

    // 中奖动画
    void awardAnimation(const std::string& strAward);

    // 发牌动画
    void dealCardAnimation(const std::string& strPlayerCard,const std::string& strBankerCard);

    //允许下注
    void allowPlaying(bool isAllow);

    //飞筹码
    bool addChip(int area, int money);

    //清空筹码
    void clearChips();



    ///--------->成员变量<---------
private:
    Ui::BaccaratUI *ui;
    int m_freeTime;                 // 空闲时间
    int m_betTime;                  // 下注时间
    int m_openTime;                 // 开奖时间
    int m_remainTime;               // 剩余时长
    int m_curState;                 // 当前状态
    int m_winner;                   // 输赢 0：庄赢 1:闲赢 2:和 (并非及时播放声音，为此缓存输赢结果)

    qint64 m_BetGolds;              // 下注金额
    qint64 m_PlayerGold;            // 玩家金币
    QString m_gameInfo;

    bool m_animatIsFinish;          // 动画
    BaccaratLogic*  m_logic;        // 游戏逻辑

    QTimer* m_timer;                // 状态 定时器
    QLabel* m_select;               // 选中筹码
    QStandardItemModel* m_modelPlayers; // 玩家列表
    QParallelAnimationGroup* m_animationGroup; //中奖动画
    // QSequentialAnimationGroup* m_cardGroup;//发牌动画
};

#endif // BACCARATUI_H
