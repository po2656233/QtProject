#include "landlordui.h"
#include "ui_landlordui.h"
#include "base/network/handlemsg.h"
#include "base/global.h"
#include "base/gameunit.h"
#include "base/baseui/uicard.h"


//proto
#include "landLords.pb.h"


//Qt
#include <QBitmap>
#include <QDebug>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>

LandlordUI::LandlordUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::LandlordUI)
{
    ui->setupUi(this);
    m_userID = 0;
    m_curState = SubGameSenceFree;
    initUI();
}

LandlordUI::~LandlordUI()
{
    delete ui;
}

void LandlordUI::enterScene(int scene, const std::string &data)
{
    go::GameLandLordsEnter enter;
    if(enter.IsInitialized() && enter.ParseFromArray(data.c_str(), data.length()))
    {
        // qint64 curtime = QDateTime::currentDateTime().toSecsSinceEpoch();//.toMSecsSinceEpoch();
        m_curState = scene;
        QString strSceneName;
        switch (scene) {
        case SubGameSenceStart:
            strSceneName = "开始";
            this->readyState();
            break;
        case SubGameSencePlaying:
            strSceneName = "下注";
            this->startState();
            break;
        case SubGameSenceOver:
            strSceneName = "结算";
            this->overState();
            break;
        case SubGameSenceFree://当前无用(准备阶段)
            strSceneName = "空闲";
            this->enterState();
            break;
        default:
            break;
        }

        m_userID = enter.userid();
        const go::UserList& list = enter.players();
        for(int i = 0; i < list.allinfos_size(); i++)
        {// 玩家列表
            const  go::PlayerInfo& info = list.allinfos(i);
            if(qint64(info.userid()) == m_userID)
            {// 个人信息
                ui->label_name->setText(tr("名字:")+info.name().c_str());
                m_playerGold = info.gold();

                //显示的时候,需除以100
                ui->label_gold->setText(tr("金币:")+QString::number(m_playerGold/100,'f',2));
            }
        }


    }

}
void LandlordUI::changeState(int state, const std::string &data)
{

    switch (state) {
    case SubGameStateStart:
        doStateStart(data);
        this->startState();
        break;
    case SubGameStatePlaying:
        doStatePlaying(data);
        break;
    case SubGameStateOver:
        doStateOver(data);
        this->overState();
        break;
    default:
        break;
    }
}


bool LandlordUI::gameHandle(int code, const std::string &data)
{

    switch (code) {
    case SubGameFrameReady:
    {

        go::GameReady ready;
        if(ready.IsInitialized() && ready.ParseFromArray(data.c_str(), data.length()))
        {
            if (qint64(ready.userid()) == m_userID)
            {// 是否自己准备
                m_curState = SubGameFrameReady;
            }
        }
        return true;
    }
    case SubGameFrameResult:
    {
        go::GameResult result;
        if(result.IsInitialized() && result.ParseFromArray(data.c_str(), data.length()))
        {
            if (0 == result.flag())
            {// 出牌成功
                qDebug()<<"出牌成功";
                ui->handCards->clearUp();
            }else{
                ui->handCards->doNot();
                QMessageBox::warning(this,tr("提示"),tr("无效牌型"));
                qDebug()<<"出牌失败";
            }
        }

        return true;
    }
    default:
        break;
    }
    qDebug()<<"无效---";
    return false;
}

bool LandlordUI::updateInfo(int code, const std::string &data)
{
    Q_UNUSED(code)
    Q_UNUSED(data)
    return false;
}


void LandlordUI::readyState()
{
    ui->pushButton_Hint->setText("已准备");
    ui->pushButton_Hint->show();
    ui->pushButton_NoOut->hide();
    ui->pushButton_Out->hide();
    resetTabel();
}

void LandlordUI::enterState()
{
    ui->pushButton_Hint->setText("准备");
    ui->pushButton_Hint->show();
    ui->pushButton_NoOut->hide();
    ui->pushButton_Out->hide();
}

void LandlordUI::startState()
{
    ui->pushButton_Hint->setText("提示");
    ui->pushButton_Hint->show();
    ui->pushButton_NoOut->setText("不出");
    ui->pushButton_NoOut->show();
    ui->pushButton_Out->setText("出牌");
    ui->pushButton_Out->show();
}

void LandlordUI::overState()
{
    ui->pushButton_Hint->hide();
    ui->pushButton_NoOut->hide();
    ui->pushButton_Out->hide();
}

bool LandlordUI::doStateStart(const string &data)
{
    resetTabel();
    go::GameLandLordsBegins beginInfo;
    if(beginInfo.IsInitialized() && beginInfo.ParseFromArray(data.c_str(), data.length()))
    {

        std::string strBottomCard = beginInfo.cardsbottom();
        std::string strHandCard = beginInfo.cardshand();
        for(size_t i = 0; i < 17; i++)
        {
            ui->handCards_player2->addCard(0);
            ui->handCards_player3->addCard(0);
        }

        for(size_t i = 0; i < strBottomCard.length(); i++)
        {
            ui->handCards_bottom->addCard(strBottomCard.at(i));
        }

        for(size_t i = 0; i < strHandCard.length(); i++)
        {
            ui->handCards->addCard(strHandCard.at(i),i);
        }
        return true;
    }
    return false;
}

bool LandlordUI::doStatePlaying(const string &data)
{
    go::GameLandLordsOperate operateInfo;
    if(operateInfo.IsInitialized() && operateInfo.ParseFromArray(data.c_str(), data.length()))
    {

        ui->handCards_out->clearCards();
        int code = operateInfo.code(); //解析用
        std::string strOutCard = operateInfo.cards();
        for(size_t i = 0; i < strOutCard.length(); i++)
        {
            ui->handCards_out->addCard(strOutCard.at(i));
        }
        return true;
    }
    return false;
}

bool LandlordUI::doStateOver(const string &data)
{

}


void LandlordUI::paintEvent(QPaintEvent *)
{
    //确保脚本生效
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void LandlordUI::initUI()
{
    readyState();
}

void LandlordUI::resetTabel()
{
    enterState();
    ui->handCards_bottom->clearCards();
    ui->handCards_bottom->setState(2);
    ui->handCards_bottom->startPos(10,0);
    ui->handCards_bottom->setCardWidth(60);
    ui->handCards_bottom->setCardHeight(40);


    ui->handCards_out->clearCards();
    ui->handCards_out->setState(2);
    ui->handCards_out->startPos(100,60);
    ui->handCards_out->setCardWidth(80);
    ui->handCards_out->setCardHeight(60);

    ui->handCards_player2->clearCards();
    ui->handCards_player2->setState(2);
    ui->handCards_player2->startPos(0,10);
    ui->handCards_player2->setCardWidth(40);
    ui->handCards_player2->setCardHeight(30);
    ui->handCards_player2->setDirect(UIHandCard::Direct::OverlayHalf);

    ui->handCards_player3->clearCards();
    ui->handCards_player3->setState(2);
    ui->handCards_player3->startPos(0,10);
    ui->handCards_player3->setCardWidth(40);
    ui->handCards_player3->setCardHeight(30);
    ui->handCards_player3->setDirect(UIHandCard::Direct::OverlayHalf);

    ui->handCards->clearCards();
    ui->handCards->startPos(100,60);
    ui->handCards->setCardWidth(80);
    ui->handCards->setCardHeight(60);
    ui->handCards->setDirect(UIHandCard::Direct::OverlayHalf);
}

void LandlordUI::faPai()
{
    //牌的背面放置在牌桌中心區域

}

void LandlordUI::on_pushButton_Hint_clicked()
{
    HandleMsg::GetInstance()->ReqReady(m_userID, true);
}

void LandlordUI::on_pushButton_NoOut_clicked()
{
    ui->handCards->doNot();
}

void LandlordUI::on_pushButton_Out_clicked()
{
    //出牌消息
    std::string strOutCards = ui->handCards->outCards();
    if(strOutCards.empty())return;

    HandleMsg::GetInstance()->ReqPlayCard(0,strOutCards.c_str(), strOutCards.length(),NULL,0);

}
