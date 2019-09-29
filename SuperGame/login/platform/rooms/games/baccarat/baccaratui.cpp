#include "baccaratui.h"
#include "ui_baccaratui.h"
#include "base/network/handlemsg.h"
#include "baccaratlogic.h"

#include "base/global.h"
#include "base/baseui/uicard.h"


// proto
#include <baccarat.pb.h>
#include <gamecomm.pb.h>

// Qt
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QDialog>
#include <QHBoxLayout>
#include <QEnterEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QStandardItem>
#include <QSound>

BaccaratUI::BaccaratUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::BaccaratUI)
{
    ui->setupUi(this);

    //成员构建
    m_winner = -1;
    m_curState = 0;
    m_remainTime = 0;

    m_freeTime = 0;
    m_betTime = 0;
    m_openTime = 0;
    m_BetGolds = 0;
    m_PlayerGold = 0;


    m_animatIsFinish = true;
    m_timer = new QTimer;
    m_select = nullptr;
    m_animationGroup = new QParallelAnimationGroup;
    m_modelPlayers = new QStandardItemModel(ui->listView);
    m_logic = new BaccaratLogic();

    //初始化信息
    init();

    //定时器触发
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

}

BaccaratUI::~BaccaratUI()
{
    if(m_logic) delete m_logic;


    if(m_timer)
    {
        m_timer->deleteLater();
    }
    if(m_animationGroup)
    {
        m_animationGroup->deleteLater();
    }
    if(m_modelPlayers)
    {
        m_modelPlayers->deleteLater();
    }

    delete ui;
}


void BaccaratUI::enterScene(int scene,const std::string &data)
{
    QString strSceneName = "";
    go::GameBaccaratEnter enter;
    if(enter.IsInitialized() && enter.ParseFromArray(data.c_str(), data.length()))
    {
        ///展示【个人信息】
        const go::PlayerInfo& info = enter.userinfo();
        //昵称
        ui->label_name->setText(tr("名字:")+info.name().c_str());
        //金币
        m_PlayerGold = info.gold();
        ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold/100,'f',2));        //显示金币的时候,需除以100
        //筹码(仅显示前五个筹码)
        if(4 < enter.chips_size())
        {
            ui->label_Clip1->setToolTip( QString::number(enter.chips(0)) );
            ui->label_Clip2->setToolTip( QString::number(enter.chips(1)) );
            ui->label_Clip3->setToolTip( QString::number(enter.chips(2)) );
            ui->label_Clip4->setToolTip( QString::number(enter.chips(3)) );
            ui->label_Clip5->setToolTip( QString::number(enter.chips(4)) );
        }

        // 各个阶段时长
        m_freeTime = enter.freetime() - 1;
        m_betTime = enter.bettime() - 1;
        m_openTime = enter.opentime() - 1;

        qint64 curtime = QDateTime::currentDateTime().toSecsSinceEpoch();//.toMSecsSinceEpoch();
        m_curState = scene;
        switch (scene) {
        case SubGameSenceStart:\
        {
            strSceneName = "开始";
            m_remainTime = m_freeTime;
            playSound(0);
        }
            break;
        case SubGameSencePlaying:
        {
            allowPlaying(true);
            strSceneName = "下注";
            m_remainTime = m_betTime;
            playSound(1);
        }
            break;
        case SubGameSenceOver:
        {
            strSceneName = "结算";
            m_remainTime = m_openTime;
            playSound(2);
        }
            break;
        case SubGameSenceFree://当前无用
            strSceneName = "空闲";
            playSound(0);
            break;
        default:
            break;

        }
        m_remainTime = m_remainTime -(curtime- enter.timestamp());
        //启动定时器
        m_timer->stop();
        m_timer->start( 0 < m_remainTime?m_remainTime:1);
        m_timer->setInterval(1000);

        //提示信息
        ui->label_scrollText->setWords(tr("欢迎来到 百家乐!"));
        ui->label_State->setText(tr("状态:")+strSceneName);
        qDebug()<<QTime::currentTime()<<enter.timestamp()<<"百家乐的当前场景:"<<scene<<curtime<<strSceneName<< m_remainTime;
    }

}

void BaccaratUI::changeState(int state,const std::string &data)
{
    //保留当前状态
    m_curState = state;

    //状态解析
    switch (state) {
    case SubGameStateStart:
        doStateStart(data);
        break;
    case SubGameStatePlaying:
        doStatePlaying(data);
        break;
    case SubGameStateOver:
        doStateOver(data);
        break;
    default:
        break;
    }
}


bool BaccaratUI::gameHandle(int code,const std::string &data)
{
    
    QString strFrameName = "";

    switch (code) {
    case SubGameFrameStart:
        return doFrameStart(data);
    case SubGameFramePlaying:
        return doFramePlaying(data);
    case SubGameFrameBetResult:
        return doFrameBetResult(data);
    case SubGameFrameOver:
        return doFrameOver(data);
    case SubGameFrameCheckout:
        return doFrameCheckout(data);
    case SubGameFrameSetHost:
        strFrameName = "定庄";
        break;
    case SubGameFrameHost:
        strFrameName = "抢庄";
        break;
    case SubGameFrameSuperHost:
        strFrameName = "超级抢庄";
        break;
    case SubGameFrameResult:
        strFrameName = "结果信息";
        break;
    default:
        break;
    }
    
    qDebug()<<QTime::currentTime()<<"百家乐 服务端反馈:"<<code<<strFrameName;
    return true; //临时
    
}

bool BaccaratUI::updateInfo(int code, const std::string &data)
{
    switch (code) {
    case GameUpdatePlayerList://广播玩家列表
        return doUpdatePlayerList(data);
    default:
        break;
    }
    return false;
}


bool BaccaratUI::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == event->MouseButtonDblClick ) return true;

    if(m_curState == SubGameStatePlaying)
    {
        QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);

        if(mouseEvent->type() == mouseEvent->MouseButtonPress ){

            if(watched==ui->label_Clip1 || watched==ui->label_Clip2 || watched==ui->label_Clip3 || watched==ui->label_Clip4 || watched==ui->label_Clip5 )
            {//选中筹码
                if(m_select) m_select->setStyleSheet("background-color:rgb(206, 206, 206);");
                m_select = qobject_cast<QLabel*>(watched);
                m_select->setStyleSheet("background-color:red");
            }
            else if(m_select)
            {//下注区域点击
                allowPlaying(true);
                QLabel* label = qobject_cast<QLabel*>(watched);
                if(label)
                {// 发送下注信息
                    onPlayBetting(m_logic->GetArea(label->text().toStdString()), m_select->toolTip().toInt(nullptr,10));
                }
            }

        }
    }

    return QWidget::eventFilter(watched, event);
}

void BaccaratUI::onPlayBetting(int area, qint64 money)
{
    if(0xFF == area)return;

    addChip(area,money);
    HandleMsg::GetInstance()->ReqPlayBet(area, money*100 );
}

bool BaccaratUI::doStateStart(const std::string &data)
{
    Q_UNUSED(data)
    clearChips();
    allowPlaying(false);
    m_remainTime = m_freeTime;
    m_select = nullptr;

    ui->label_GameInfo->setText("玩家游戏信息");
    ui->label_banker->setText(tr("<p><span style=\" font-size:18pt; font-weight:600; color:#ff0000;\">庄</span></p>"));
    ui->label_player->setText(tr("<p><span style=\" font-size:18pt; font-weight:600; color:#5500ff;\">闲</span></p>"));

    ui->label_scrollText->setWords( "请准备,即将进入下注阶段" );


    ui->label_State->setText(tr("状态: 开始"));
    qDebug()<<QTime::currentTime()<<"百家乐的当前状态: 开始";

    return true;
}

bool BaccaratUI::doStatePlaying(const std::string &data)
{
    Q_UNUSED(data)
    m_BetGolds = 0;
    m_remainTime = m_betTime;
    ui->label_scrollText->setWords( "开始下注啦!" );
    playSound(1);

    ui->label_State->setText(tr("状态: 下注"));
    qDebug()<<QTime::currentTime()<<"百家乐的当前状态: 下注";
    return true;
}

bool BaccaratUI::doStateOver(const std::string &data)
{
    Q_UNUSED(data)
    allowPlaying(false);
    m_remainTime = m_openTime;
    playSound(2);

    ui->label_State->setText(tr("状态: 结算"));
    qDebug()<<QTime::currentTime()<<"百家乐的当前状态: 结算";
    return true;
}

bool BaccaratUI::doFrameStart(const std::string &data)
{
    Q_UNUSED(data)
    return false;
}

bool BaccaratUI::doFramePlaying(const std::string &data)
{
    go::GameBet bet;
    if(bet.IsInitialized() && bet.ParseFromArray(data.c_str(),data.length()))
    {
        qDebug()<<"其他玩家 下注区域:"<<bet.betarea()<<" 下注分数:"<<bet.betscore()/100;
        return true;
    }
    return false;
}

bool BaccaratUI::doFrameBetResult(const std::string &data)
{
    go::GameBetResult betResult;
    if(betResult.IsInitialized() && betResult.ParseFromArray(data.c_str(),data.length()))
    {
        qDebug()<<"下注结果 状态:"<<betResult.state()<<" 提示:"<<betResult.hints().c_str() ;
        if(0 == betResult.state())
        {

            m_BetGolds += betResult.betscore();
            m_PlayerGold -= betResult.betscore();
            ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold/100,'f',2));
            QString strInfo = tr("\n区域:%1 下注:%2").arg( m_logic->GetAreaText( betResult.betarea()).c_str() ).arg( betResult.betscore()/100);
            ui->label_GameInfo->setText(strInfo);

            //下注的声音
            playSound(7);
        }
        return true;
    }
    return false;
}

bool BaccaratUI::doFrameOver(const std::string &data)
{
    go::GameBaccaratOver over;
    if(over.IsInitialized() && over.ParseFromArray(data.c_str(),data.length()))
    {
        qDebug()<<"成功解析";
        std::string strAwardarea = over.awardarea();
        std::string strPlayercard = over.playercard();
        std::string strBankercard = over.bankercard();
        // 点数比较
        int playCount = 0, bankCount = 0;
        m_logic->CompareCard(strPlayercard.c_str(), strBankercard.c_str(), playCount, bankCount);

        //显示点数
        ui->label_banker->setText(tr("<html><head/><body><p><span style=\" font-size:18pt; font-weight:600; color:#ff0000;\">庄:%1点</span></p></body></html>").arg(bankCount));
        ui->label_player->setText(tr("<html><head/><body><p><span style=\" font-size:18pt; font-weight:600; color:#5500ff;;\">闲:%1点</span></p></body></html>").arg(playCount));

        // 发牌动画
        dealCardAnimation(strPlayercard,strBankercard);

        // 中奖区域动画
        awardAnimation(strAwardarea);

        return true;
    }
    return false;

}

bool BaccaratUI::doFrameCheckout(const std::string &data)
{
    m_curState = SubGameStateOver;
    go::GameBaccaratCheckout checkOut;
    if(checkOut.IsInitialized() && checkOut.ParseFromArray(data.c_str(),data.length()))
    {
        qDebug()<<"成功解析";
        checkOut.acquire();//玩家得分
        // 金币刷新
        m_remainTime = m_openTime;
        m_PlayerGold += checkOut.acquire();
        ui->label_gold->setText(tr("金币:")+QString::number(m_PlayerGold/100,'f',2));

        m_BetGolds = checkOut.acquire() - m_BetGolds;
        // 显示中奖信息
        if(0 <= m_BetGolds)
        {
            ui->label_GameInfo->setText( tr("\n\n本轮结束,赢了:%2金币\n").arg(m_BetGolds/100));
        }else{
            ui->label_GameInfo->setText( tr("\n\n本轮结束,输了:%2金币\n").arg((-m_BetGolds)/100));
        }
        return true;
    }
    return false;
}

bool BaccaratUI::doFrameSetHost(const std::string &data)
{
    Q_UNUSED(data)
    return true;
}

bool BaccaratUI::doFrameHost(const std::string &data)
{
    Q_UNUSED(data)
    return true;
}

bool BaccaratUI::doFrameSuperHost(const std::string &data)
{
    Q_UNUSED(data)
    return true;
}

bool BaccaratUI::doFrameResult(const std::string &data)
{
    Q_UNUSED(data)
    return true;
}

bool BaccaratUI::doUpdatePlayerList(const std::string &data)
{
    go::UserList userList;
    if(userList.IsInitialized() && userList.ParseFromArray(data.c_str(),data.length()))
    {
        qDebug()<<"更新玩家信息 成功解析";
        m_modelPlayers->clear();
        for(int i = 0; i < userList.allinfos_size(); i++)
        {// 玩家列表
            m_modelPlayers->appendRow(new QStandardItem(userList.allinfos(i).name().c_str()));
        }
        return true;
    }
    return false;

}

void BaccaratUI::onTimeout()
{


    static bool bBlick = true;
    if (m_curState == SubGameStatePlaying && 0 < m_remainTime)
    {// 下注状态 文本进行闪烁提示
        if(bBlick)
        {
            ui->label_State->setStyleSheet("font-size:8pt;font-weight:200; color:#ff0000;");
        }
        else
        {
            ui->label_State->setStyleSheet("font-size:10pt;font-weight:400; color:#ff0000;");
        }
        bBlick = !bBlick;


    }
    else
    {
        ui->label_State->setStyleSheet("");
    }

    //报输赢
    if (m_curState == SubGameStateOver)
    {
        if(2 == m_remainTime && 0<=m_winner) playSound(3+m_winner);;
    }else
    {  // 不开奖时隐藏扑克
        ui->poker_1->hide();
        ui->poker_2->hide();
        ui->poker_3->hide();
        ui->poker_4->hide();
        ui->poker_5->hide();
        ui->poker_6->hide();
    }




    // 倒计时显示
    if(0 < m_remainTime && m_remainTime < 10)
    {
        QString strPicture = QString(":/img/number/ddz_result_base_winstrea_%1.png").arg(m_remainTime);
        ui->label_Number->setPixmap(QPixmap(strPicture));
        m_remainTime--;
    }else if(10 <= m_remainTime)
    {
        m_remainTime--;
    }
    else{
        ui->label_Number->setPixmap(QPixmap(":/img/number/ddz_result_base_winstrea_0.png"));
    }


}

void BaccaratUI::awardAnimation(const std::string &award)
{

    m_animationGroup->clear();
    QString strInfo = tr("\n开奖区域:");
    QString strOpenArea = tr("");
    for(size_t i = 0; i < award.length(); i++)
    {
        unsigned char card = award.at(i);
        if(1 == card)
        {
            strOpenArea = m_logic->GetAreaText(i).c_str();
            if(strOpenArea.contains("庄"))
            {
                m_winner = 0;
            }
            else if(strOpenArea.contains("闲"))
            {
                m_winner = 1;
            }
            else if(strOpenArea.contains("平"))
            {
                m_winner = 2;
            }


            strInfo +=tr(" ")+ strOpenArea;
            UIBetArea *pArea = getArea(i);
            if(nullptr != pArea)
            {

                QPropertyAnimation *animation = new QPropertyAnimation(pArea,"Color");
                animation->setDuration(1000);
                animation->setKeyValueAt(0, QColor(247, 0, 255,255));
                animation->setKeyValueAt(0.5, QColor(92, 0, 255,255));
                animation->setKeyValueAt(1, QColor(247, 180, 255,255));
                animation->setLoopCount(7);
                //                enum Type {动画类型
                //                    Linear,
                //                    InQuad, OutQuad, InOutQuad, OutInQuad,
                //                    InCubic, OutCubic, InOutCubic, OutInCubic,
                //                    InQuart, OutQuart, InOutQuart, OutInQuart,
                //                    InQuint, OutQuint, InOutQuint, OutInQuint,
                //                    InSine, OutSine, InOutSine, OutInSine,
                //                    InExpo, OutExpo, InOutExpo, OutInExpo,
                //                    InCirc, OutCirc, InOutCirc, OutInCirc,
                //                    InElastic, OutElastic, InOutElastic, OutInElastic,
                //                    InBack, OutBack, InOutBack, OutInBack,
                //                    InBounce, OutBounce, InOutBounce, OutInBounce,
                //                    InCurve, OutCurve, SineCurve, CosineCurve,
                //                    BezierSpline, TCBSpline, Custom, NCurveTypes
                //                };
                animation->setEasingCurve(QEasingCurve::InOutQuad);
                m_animationGroup->addAnimation( animation );

                qDebug()<<"有动画";
            }

        }
    }

    ui->label_scrollText->setWords( strInfo );
    m_animationGroup->start();
}

void BaccaratUI::dealCardAnimation(const std::string &strPlayerCard, const std::string &strBankerCard)
{
    if(!m_animatIsFinish)return;

    //if(m_cardGroup)m_cardGroup = new QSequentialAnimationGroup;

    // 设置张牌
    qDebug()<<"子集合个数:";
    ui->poker_1->show();
    ui->poker_2->show();
    ui->poker_3->show();
    ui->poker_4->show();
    ui->poker_5->show();
    ui->poker_6->show();

    ui->poker_1->setCard(strBankerCard.at(0));
    ui->poker_2->setCard(strBankerCard.at(1));
    ui->poker_3->setCard(strBankerCard.at(2));

    ui->poker_4->setCard(strPlayerCard.at(0));
    ui->poker_5->setCard(strPlayerCard.at(1));
    ui->poker_6->setCard(strPlayerCard.at(2));


}

void BaccaratUI::clearChips()
{
    ui->label_TDP->clearALL();
    ui->label_XDZ->clearALL();
    ui->label_Xian->clearALL();
    ui->label_XTW->clearALL();
    ui->label_ZDZ->clearALL();
    ui->label_Zhuang->clearALL();
    ui->label_ZTW->clearALL();
    ui->label_Ping->clearALL();

    //还原筹码
    if(m_select) m_select->setStyleSheet("background-color:rgb(206, 206, 206);");
}

void BaccaratUI::init()
{

    //玩家列表
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView->setModel(m_modelPlayers);


    //初始化牌
    ui->poker_1->hide();
    ui->poker_2->hide();
    ui->poker_3->hide();
    ui->poker_4->hide();
    ui->poker_5->hide();
    ui->poker_6->hide();


    //初始化筹码  可控
    ui->label_Clip1->installEventFilter(this);
    ui->label_Clip2->installEventFilter(this);
    ui->label_Clip3->installEventFilter(this);
    ui->label_Clip4->installEventFilter(this);
    ui->label_Clip5->installEventFilter(this);

    //开奖区域
    ui->label_Zhuang->installEventFilter(this);
    ui->label_Ping->installEventFilter(this);//
    ui->label_Xian->installEventFilter(this);
    ui->label_ZDZ->installEventFilter(this);//庄对子
    ui->label_TDP->installEventFilter(this);//同点平
    ui->label_XDZ->installEventFilter(this);
    ui->label_XTW->installEventFilter(this);
    ui->label_ZTW->installEventFilter(this);

    //初始化声音
    clearSound();
    addSound("qrc:/sound/baccarat/countdown.mp3");//0
    addSound("qrc:/sound/baccarat/startbet.mp3");//1
    addSound("qrc:/sound/baccarat/stopbet.mp3");//2
    addSound("qrc:/sound/baccarat/gameover_banker_win.mp3");//3
    addSound("qrc:/sound/baccarat/gameover_player_win.mp3");//4
    addSound("qrc:/sound/baccarat/gameover_tie.mp3");//5
    addSound("qrc:/sound/baccarat/sendcard.mp3");//6
    addSound("qrc:/sound/baccarat/bet.mp3");//7
    setSoundVolume(80);
}

UIBetArea *BaccaratUI::getArea(int area)
{
    UIBetArea *pArea = nullptr;
    BaccaratLogic::Area enArea = static_cast<BaccaratLogic::Area>(area);
    switch (enArea) {
    case BaccaratLogic::Area::XIAN:
        pArea = ui->label_Xian;
        break;
    case BaccaratLogic::Area::PING:
        pArea = ui->label_Ping;
        break;
    case BaccaratLogic::Area::ZHUANG:
        pArea = ui->label_Zhuang;
        break;
    case BaccaratLogic::Area::XIAN_TIAN:
        pArea = ui->label_XTW;
        break;
    case BaccaratLogic::Area::ZHUANG_TIAN:
        pArea = ui->label_ZTW;
        break;
    case BaccaratLogic::Area::TONG_DUI:
        pArea = ui->label_TDP;
        break;
    case BaccaratLogic::Area::XIAN_DUI:
        pArea = ui->label_XDZ;
        break;
    case BaccaratLogic::Area::ZHUANG_DUI:
        pArea = ui->label_ZDZ;
        break;
    default:
        qDebug()<<"无效区域";
        break;
    }
    return pArea;
}

void BaccaratUI::allowPlaying(bool isAllow)
{
    ui->label_TDP->allowClick(isAllow);
    ui->label_XDZ->allowClick(isAllow);
    ui->label_Xian->allowClick(isAllow);
    ui->label_XTW->allowClick(isAllow);
    ui->label_ZDZ->allowClick(isAllow);
    ui->label_Zhuang->allowClick(isAllow);
    ui->label_ZTW->allowClick(isAllow);
    ui->label_Ping->allowClick(isAllow);
}

bool BaccaratUI::addChip(int area, int money)
{
    UIBetArea* pArea = getArea(area);
    if(pArea)
    {
        qDebug()<<"---->下注金额<---- :"<<money;
        switch (money) {
        case 1:
            pArea->setChipType(NSType::enChip::OneRMB);
            break;
        case 2:
            pArea->setChipType(NSType::enChip::TwoRMB);
            break;
        case 5:
            pArea->setChipType(NSType::enChip::FiveRMB);
            break;
        case 10:
            pArea->setChipType(NSType::enChip::TwentyRMB);
            break;
        case 25:
            pArea->setChipType(NSType::enChip::TwentyRMB);
            break;
        case 50:
            pArea->setChipType(NSType::enChip::FiftyRMB);
            break;
        case 100:
            pArea->setChipType(NSType::enChip::HundredRMB);
            break;
        default:
            pArea->setChipType(NSType::enChip::ZeroRMB);
            return false;
        }

    }
    return true;
}
