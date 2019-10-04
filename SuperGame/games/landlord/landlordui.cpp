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
    {

        resetTabel();
        go::GameLandLordsBegins beginInfo;
        if(beginInfo.IsInitialized() && beginInfo.ParseFromArray(data.c_str(), data.length()))
        {

            std::string strBottomCard = beginInfo.cardsbottom();
            std::string strHandCard = beginInfo.cardshand();

            //            for(int i = 0; i < strHandCard.length(); i++)
            //            {
            //                Poker* poker = new Poker;
            //                poker->setCard(strHandCard.at(i));
            //                m_layoutCards->addWidget(poker);
            //            }
            for(size_t i = 0; i < strBottomCard.length(); i++)
            {
//                UICard* poker = qobject_cast<UICard*> (ui->horizontalLayout_2->itemAt(i)->widget());
//                if(poker)
//                {
//                    poker->clear();
//                    poker->setCard(strBottomCard.at(i));

//                }
            }

            for(size_t i = 0; i < strHandCard.length(); i++)
            {
                ui->handCards->addCard(strHandCard.at(i),30,i);
//                UICard* poker = qobject_cast<UICard*> (ui->groupBox_player3->children().at(i));
//                if(poker)
//                {
//                    poker->setCard(strHandCard.at(i));
//                }
            }
            ui->handCards->show();
            qDebug()<<"底牌:"<<strBottomCard.c_str();


        }
    }
        this->startState();
        break;
    case SubGameStatePlaying:
        this->startState();
        break;
    case SubGameStateOver:
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
                readyState();
                m_curState = SubGameFrameReady;
            }
        }
        return true;
    }

        break;
    default:
        break;
    }
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

bool LandlordUI::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);
    if(mouseEvent->type() == mouseEvent->MouseButtonPress ){
        UICard* selectCard = qobject_cast<UICard*>(watched);
        if(selectCard)
        {
            const QRect& rect = selectCard->geometry();
            int state = selectCard->indent();
            if(0 == state)
            {
                selectCard->setGeometry(rect.x(),rect.y()-20,rect.width(),rect.height());
                selectCard->setIndent(1);
            }
            else if(1 == state)
            {
                selectCard->setGeometry(rect.x(),rect.y()+20,rect.width(),rect.height());
                selectCard->setIndent(0);
            }
            qDebug()<<"选中了";
        }
    }
    return QWidget::eventFilter(watched,event);
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
    UICard *pieces = nullptr;
    int nCount = ui->groupBox_player3->children().count();
    QObjectList objs = ui->groupBox_player3->children();

    for(int i = 0; i<nCount; i++)
    {
        pieces = qobject_cast<UICard*>( objs.at(i) );
        if(pieces){
            pieces->installEventFilter(this);
            pieces->setIndent(0);
        }
    }
    ui->handCards->startPos(ui->groupBox_player1->x()+20, ui->groupBox_player1->y()+10);
    readyState();

    //    QPixmap mypixmap;
    //    ui->pushButton->text().clear();
    //    mypixmap.load(":/img/landlord/item_chupai.png");
    //    ui->pushButton->setMask(mypixmap.mask());
    //    ui->pushButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    //    ui->pushButton->setIcon(mypixmap);
    //    ui->pushButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

}

void LandlordUI::resetTabel()
{
    enterState();
    UICard *pieces = nullptr;

    int nCount = ui->groupBox_player3->children().count();
    QObjectList objs = ui->groupBox_player3->children();



    //重新整理手牌
    int posX = 11;
    int posY = 33;
    for(int i = 0; i<nCount; i++)
    {
        pieces = qobject_cast<UICard*>( objs.at(i) );
        if(pieces){
            pieces->setIndent(0);
            const QRect& rect = pieces->geometry();

            pieces->setGeometry(posX, posY, rect.width(), rect.height());
            posX += 55;
            pieces->show(); // 需要重新show出来
        }
    }

    //清空出牌区域
    UICard *outCard = nullptr;
    QLayoutItem* pItem = nullptr;
//    for(int i = 0; i< ui->horizontalLayout_out->count(); i++)
//    {
//        pItem = ui->horizontalLayout_out->itemAt(i);
//        if(pItem)
//        {
//            outCard = qobject_cast<UICard*>(pItem->widget());
//            if(outCard)     outCard->hide();
//        }
//    }


    //    QLayoutItem *child;
    //    while ((child = m_layoutCards->takeAt(0)) != 0) {
    //        child->widget()->deleteLater();
    //        delete child;
    //    }
}

void LandlordUI::faPai()
{
    //牌的背面放置在牌桌中心區域

}

void LandlordUI::on_pushButton_Hint_clicked()
{

    HandleMsg::GetInstance()->ReqReady(m_userID, true);
    if(m_curState == SubGameSenceFree)
    {//发送准备
        //S_GameKindID
        HandleMsg::GetInstance()->ReqReady(m_userID, true);
    }else{//提示

    }
}

void LandlordUI::on_pushButton_NoOut_clicked()
{

}

void LandlordUI::on_pushButton_Out_clicked()
{



    //出牌消息
    QString strOutCards = ui->handCards->outCards();
    if(strOutCards.isEmpty())return;
    HandleMsg::GetInstance()->ReqPlayCard(0,strOutCards.toStdString().c_str(),strOutCards.length(),NULL,0);

//    //牌值紧凑
//    int posX = 11;
//    for(int i = 0; i<nCount; i++)
//    {
//        pieces = qobject_cast<UICard*>(objs.at(i));
//        if(pieces){
//            if(0 == pieces->indent())
//            {
//                const QRect& rect = pieces->geometry();
//                pieces->setGeometry(posX, rect.y(), rect.width(), rect.height());
//                posX += 55;
//            }
//        }
//    }




}
