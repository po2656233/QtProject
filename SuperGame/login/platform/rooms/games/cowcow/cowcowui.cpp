#include "cowcowui.h"
#include "ui_cowcowui.h"

#include "base/global.h"
#include "base/network/handlemsg.h"
#include "base/gameunit.h"
#include "base/baseui/uicard.h"


// proto
#include <cowcow.pb.h>
#include <gamecomm.pb.h>

// Qt
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QDialog>
#include <QHBoxLayout>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QStandardItem>

CowcowUI::CowcowUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::CowcowUI)
{
    ui->setupUi(this);

    //测试用
    m_curState = 0;
    m_remainTime = 0;

    m_freeTime = 0;
    m_betTime = 0;
    m_openTime = 0;
    m_BetGolds = 0;
    m_PlayerGold = 0;


    m_animatIsFinish = true;
    m_timer = new QTimer(this);
    m_select = nullptr;
    m_animationGroup = new QParallelAnimationGroup;
    m_cardGroup = new QSequentialAnimationGroup;
    m_modelPlayers = new QStandardItemModel(ui->listView);
    m_logic = new CowcowLogic();

    init();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

CowcowUI::~CowcowUI()
{
    delete ui;
}
void CowcowUI::enterScene(int scene, const std::string &data)
{
    clearChips();
    QString strSceneName = "";
    //ui->label_scrollText->setWords(tr("欢迎来到 cowcow!"));
    go::GameCowcowEnter enter;
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

        //初始化牌值
        initCard();

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



        //启动定时器
        m_remainTime = m_remainTime -(curtime- enter.timestamp());
        m_timer->stop();
        m_timer->start( 0 < m_remainTime?m_remainTime:1 );
        m_timer->setInterval(1000);

        //提示信息
        //ui->label_scrollText->setWords(tr("欢迎来到 百家乐!"));
        //设置字体大小
        QFont ft;
        ft.setPointSize(36);
        ft.setWeight(60);
        ui->label_State->setFont(ft);

        //设置颜色
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor("#ff0000"));
        ui->label_State->setPalette(pa);
        //ui->label_State->setText(tr("<p>当前状态:<span style=\" font-size:18pt; font-weight:600; color:#ff0000;\">%1</span></p>").arg(strSceneName));

        ui->label_State->setText(tr("状态:")+strSceneName);
        qDebug()<<QTime::currentTime()<<enter.timestamp()<<"牛牛的当前场景:"<<scene<<curtime<<strSceneName<< m_remainTime;
    }



}

void CowcowUI::changeState(int state, const std::string &data)
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

bool CowcowUI::gameHandle(int code, const std::string &data)
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

    return false;
}

bool CowcowUI::updateInfo(int code, const std::string &data)
{
    switch (code) {
    case GameUpdatePlayerList://广播玩家列表
        return doUpdatePlayerList(data);
    default:
        break;
    }
    return false;
}

bool CowcowUI::eventFilter(QObject *watched, QEvent *event)
{

    if( event->type() == event->MouseButtonDblClick ) return true;

    if(m_curState == SubGameStatePlaying)
    {
        QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);
        if(mouseEvent->type() == mouseEvent->MouseButtonPress && mouseEvent->button() ==Qt::LeftButton){

            ui->label_Clip1->setStyleSheet(" background-color: #aaaaff;");
            ui->label_Clip2->setStyleSheet(" background-color: #aaaaff;");
            ui->label_Clip3->setStyleSheet(" background-color: #aaaaff;");
            ui->label_Clip4->setStyleSheet(" background-color: #aaaaff;");
            ui->label_Clip5->setStyleSheet(" background-color: #aaaaff;");

            if(watched==ui->label_Clip1)
            {
                m_select = qobject_cast<QLabel*>(watched);
                ui->label_Clip1->setStyleSheet(" background-color: rgb(255, 85, 255);");
            }else if(watched==ui->label_Clip2)
            {
                m_select = qobject_cast<QLabel*>(watched);
                ui->label_Clip2->setStyleSheet(" background-color: rgb(85, 170, 255);");
            }else if(watched==ui->label_Clip3)
            {
                m_select = qobject_cast<QLabel*>(watched);
                ui->label_Clip3->setStyleSheet(" background-color: rgb(255, 110, 53);");
            }else if(watched==ui->label_Clip4)
            {
                m_select = qobject_cast<QLabel*>(watched);
                ui->label_Clip4->setStyleSheet(" background-color: rgb(255, 180, 157);");
            }else if(watched==ui->label_Clip5)
            {
                m_select = qobject_cast<QLabel*>(watched);
                ui->label_Clip5->setStyleSheet(" background-color: rgb(54, 74, 134);");
            }else if(m_select)
            {
                allowPlaying(true);
                QLabel* label = qobject_cast<QLabel*>(watched);
                CowcowLogic::Area area = m_logic->GetArea(label->text().toStdString());
                if(CowcowLogic::Area::Empty != area)
                {
                    // 发送下注信息
                    addChip(area, m_select->toolTip().toInt(nullptr,10));
                    HandleMsg::GetInstance()->ReqPlayBet(static_cast<int>(area), m_select->toolTip().toInt(nullptr,10)*100 );
                }else
                {
                    qDebug()<<"数据异常..."<<label->text().toStdString().c_str();
                }


            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

bool CowcowUI::doStateStart(const std::string &data)
{

    clearChips();
    allowPlaying(false);
    m_remainTime = m_freeTime;
    m_select = nullptr;
    m_BetGolds = 0;

    initCard();

    ui->label_GameInfo->setText(tr("游戏信息展示栏"));

    //设置字体大小
    QFont ft;
    ft.setPointSize(36);
    ft.setWeight(60);
    ui->label_State->setFont(ft);

    //设置颜色
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#ffff33"));
    ui->label_State->setPalette(pa);
    ui->label_State->setText(tr("开始"));
    //ui->label_State->setText(tr("<span style=\" font-size:36pt; font-weight:600; color:#ffff33;\">开始</span>"));

    qDebug()<<QTime::currentTime()<<"牛牛的当前状态: 开始";
    playSound(0);
    return true;
}

bool CowcowUI::doStatePlaying(const std::string &data)
{
    m_remainTime = m_betTime;

    //设置字体大小
    QFont ft;
    ft.setPointSize(36);
    ft.setWeight(60);
    ui->label_State->setFont(ft);

    //设置颜色
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#ff0077"));
    ui->label_State->setPalette(pa);
    ui->label_State->setText(tr("下注"));
    //ui->label_State->setText(tr("<span style=\" font-size:36pt; font-weight:600; color:#ff0077;\">下注</span>"));

    qDebug()<<QTime::currentTime()<<"牛牛的当前状态: 下注";
    playSound(1);
    return true;
}

bool CowcowUI::doStateOver(const std::string &data)
{
    allowPlaying(false);
    m_remainTime = m_openTime;
    //m_select = nullptr;

    //设置字体大小
    QFont ft;
    ft.setPointSize(36);
    ft.setWeight(60);
    ui->label_State->setFont(ft);

    //设置颜色
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor("#3f0ff0"));
    ui->label_State->setPalette(pa);
    ui->label_State->setText(tr("开奖"));

    qDebug()<<QTime::currentTime()<<"牛牛的当前状态: 开奖";
    playSound(2);
    return true;
}

bool CowcowUI::doFrameStart(const std::string &data)
{
}

bool CowcowUI::doFramePlaying(const std::string &data)
{

}

bool CowcowUI::doFrameBetResult(const std::string &data)
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

bool CowcowUI::doFrameOver(const std::string &data)
{
    go::GameCowcowOver over;
    if(over.IsInitialized() && over.ParseFromArray(data.c_str(),data.length()))
    {
        qDebug()<<"开奖结果--->成功解析";

        // 点数比较
        // int playCount = 0, bankCount = 0;
        //m_logic->CompareCard(strtiancard.c_str(), strBankercard.c_str(), playCount, bankCount);

        //显示各家牌面
        const char *pCardValue = over.bankercard().c_str();
        ui->label_b1->setCard(int(pCardValue[0]));
        ui->label_b2->setCard(int(pCardValue[1]));
        ui->label_b3->setCard(int(pCardValue[2]));
        ui->label_b4->setCard(int(pCardValue[3]));
        ui->label_b5->setCard(int(pCardValue[4]));

        pCardValue = over.tiancard().c_str();
        ui->label_t1->setCard(int(pCardValue[0]));
        ui->label_t2->setCard(int(pCardValue[1]));
        ui->label_t3->setCard(int(pCardValue[2]));
        ui->label_t4->setCard(int(pCardValue[3]));
        ui->label_t5->setCard(int(pCardValue[4]));

        pCardValue = over.xuancard().c_str();
        ui->label_x1->setCard(int(pCardValue[0]));
        ui->label_x2->setCard(int(pCardValue[1]));
        ui->label_x3->setCard(int(pCardValue[2]));
        ui->label_x4->setCard(int(pCardValue[3]));
        ui->label_x5->setCard(int(pCardValue[4]));

        pCardValue = over.dicard().c_str();
        ui->label_d1->setCard(int(pCardValue[0]));
        ui->label_d2->setCard(int(pCardValue[1]));
        ui->label_d3->setCard(int(pCardValue[2]));
        ui->label_d4->setCard(int(pCardValue[3]));
        ui->label_d5->setCard(int(pCardValue[4]));

        pCardValue = over.huangcard().c_str();
        ui->label_h1->setCard(int(pCardValue[0]));
        ui->label_h2->setCard(int(pCardValue[1]));
        ui->label_h3->setCard(int(pCardValue[2]));
        ui->label_h4->setCard(int(pCardValue[3]));
        ui->label_h5->setCard(int(pCardValue[4]));


        //显示牌值
        const char *pCardResult = over.cardvalue().c_str();
        ui->label_cardVBanker->setText(tr("<span style=\" font-size:18pt; font-weight:600; color:#ff0000;\">庄:%1点</span>").arg(int(pCardResult[1])));
        ui->label_cardVTian->setText(tr("<span style=\" font-size:18pt; font-weight:600; color:rgb(0, 170, 255);\">天:%1点</span>").arg(int(pCardResult[2])));
        ui->label_cardVXuan->setText(tr("<span style=\" font-size:18pt; font-weight:600; color:rgb(0, 85, 127);\">玄:%1点</span>").arg(int(pCardResult[3])));
        ui->label_cardVDi->setText(tr("<span style=\" font-size:18pt; font-weight:600; color:rgb(170, 0, 127);\">地:%1点</span>").arg(int(pCardResult[4])));
        ui->label_cardVHuang->setText(tr("<span style=\" font-size:18pt; font-weight:600; color:rgb(255, 240, 121);\">黄:%1点</span>").arg(int(pCardResult[5])));

        // 发牌动画
        // dealCardAnimation(strtiancard,strBankercard);

        // 中奖区域动画
        std::string strAwardarea = over.awardarea();
        awardAnimation(strAwardarea);

        return true;
    }
    return false;
}

bool CowcowUI::doFrameCheckout(const std::string &data)
{
    m_curState = SubGameStateOver;
    go::GameCowcowCheckout checkOut;
    if(checkOut.IsInitialized() && checkOut.ParseFromArray(data.c_str(),data.length()))
    {
        qDebug()<<"成功解析 玩家结算->"<<checkOut.acquire()<<" 玩家下注:"<<m_BetGolds;
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


bool CowcowUI::doFrameResult(const std::string &data)
{

}

bool CowcowUI::doUpdatePlayerList(const std::string &data)
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

void CowcowUI::init()
{ 
    //界面
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView->setModel(m_modelPlayers);

    //筹码接受点击
    ui->label_Clip1->installEventFilter(this);
    ui->label_Clip2->installEventFilter(this);
    ui->label_Clip3->installEventFilter(this);
    ui->label_Clip4->installEventFilter(this);
    ui->label_Clip5->installEventFilter(this);

    //下注区域接受点击
    ui->label_tian->installEventFilter(this);//天
    ui->label_di->installEventFilter(this);//地
    ui->label_xuan->installEventFilter(this);//玄
    ui->label_huang->installEventFilter(this);//黄

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

    initCard();

}

void CowcowUI::initCard()
{
    ui->label_b1->setCard(0);
    ui->label_b2->setCard(0);
    ui->label_b3->setCard(0);
    ui->label_b4->setCard(0);
    ui->label_b5->setCard(0);

    ui->label_t1->setCard(0);
    ui->label_t2->setCard(0);
    ui->label_t3->setCard(0);
    ui->label_t4->setCard(0);
    ui->label_t5->setCard(0);

    ui->label_x1->setCard(0);
    ui->label_x2->setCard(0);
    ui->label_x3->setCard(0);
    ui->label_x4->setCard(0);
    ui->label_x5->setCard(0);

    ui->label_d1->setCard(0);
    ui->label_d2->setCard(0);
    ui->label_d3->setCard(0);
    ui->label_d4->setCard(0);
    ui->label_d5->setCard(0);

    ui->label_h1->setCard(0);
    ui->label_h2->setCard(0);
    ui->label_h3->setCard(0);
    ui->label_h4->setCard(0);
    ui->label_h5->setCard(0);

    ui->label_cardVBanker->setText(tr("牌值:"));
    ui->label_cardVTian->setText(tr("牌值:"));
    ui->label_cardVXuan->setText(tr("牌值:"));
    ui->label_cardVDi->setText(tr("牌值:"));
    ui->label_cardVHuang->setText(tr("牌值:"));
}

UIBetArea *CowcowUI::getArea(const CowcowLogic::Area& area)
{
    UIBetArea *pArea = nullptr;
    switch (area) {
    case CowcowLogic::Area::Tian:
        pArea = ui->label_tian;
        break;
    case CowcowLogic::Area::Di:
        pArea = ui->label_di;
        break;
    case CowcowLogic::Area::Xuan:
        pArea = ui->label_xuan;
        break;
    case CowcowLogic::Area::Huang:
        pArea = ui->label_huang;
        break;
    default:
        qDebug()<<"无效区域";
        break;
    }
    return pArea;
}

void CowcowUI::awardAnimation(const std::string &strAward)
{
    m_animationGroup->clear();
    QString strOpenArea("");
    QColor color;
    for(size_t i = 0; i < strAward.length(); i++)
    {
        unsigned char card = strAward.at(i);
        if(1 == card)
        {


            UIBetArea *pArea = getArea(static_cast<CowcowLogic::Area>(i));
            if(nullptr != pArea)
            {

                strOpenArea = m_logic->GetAreaText(i).c_str();
                if(strOpenArea.contains("天"))
                {
                    color = QColor(0, 170, 255);
                }
                else if(strOpenArea.contains("玄"))
                {
                    color = QColor(0, 85, 127);
                }
                else if(strOpenArea.contains("地"))
                {
                    color = QColor(170, 170, 127);
                }
                else if(strOpenArea.contains("黄"))
                {
                    color = QColor(255, 240, 121);
                }

                QPropertyAnimation *animation = new QPropertyAnimation(pArea,"Color");
                animation->setDuration(300);
                animation->setKeyValueAt(0, QColor(0, 0, 255));
                animation->setKeyValueAt(0.5, QColor(28, 28, 28));
                animation->setKeyValueAt(1, color);
                animation->setLoopCount(20);
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
                animation->setEasingCurve(QEasingCurve::InOutCirc);
                m_animationGroup->addAnimation( animation );

                qDebug()<<"有动画";
            }

        }
    }
    m_animationGroup->start();
}

void CowcowUI::dealCardAnimation(const std::string &strPlayerCard, const std::string &strBankerCard)
{

}

void CowcowUI::onTimeout()
{

    // 倒计时显示
    if(0 < m_remainTime && m_remainTime < 10)
    {
        QString strPicture = QString(":/img/number/ddz_result_base_winstrea_%1.png").arg(m_remainTime);
        ui->label_time->setPixmap(QPixmap(strPicture));
        m_remainTime--;
    }else if(10 <= m_remainTime)
    {
        m_remainTime--;
    }
    else{
        ui->label_time->setPixmap(QPixmap(":/img/number/ddz_result_base_winstrea_0.png"));
    }


}

void CowcowUI::onPlayBetting(CowcowLogic::Area area, qint64 money)
{
    if(CowcowLogic::Area::Empty == area)return;

    addChip(area,money);
    HandleMsg::GetInstance()->ReqPlayBet(static_cast<int>(area), money*100 );
}


void CowcowUI::allowPlaying(bool isAllow)
{
    ui->label_tian->allowClick(isAllow);
    ui->label_di->allowClick(isAllow);
    ui->label_xuan->allowClick(isAllow);
    ui->label_huang->allowClick(isAllow);

}


bool CowcowUI::addChip(const CowcowLogic::Area& area, int money)
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

void CowcowUI::clearChips()
{
    ui->label_tian->clearALL();
    ui->label_di->clearALL();
    ui->label_xuan->clearALL();
    ui->label_huang->clearALL();
    if(m_select) m_select->setStyleSheet("background-color:rgb(206, 206, 206);");
}
