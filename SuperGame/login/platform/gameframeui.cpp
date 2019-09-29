#include "gameframeui.h"
// local
#include "ui_gameframeui.h"
#include "base/global.h"
#include "base/network/handlemsg.h"

// Qt
#include <QGridLayout>
#include <QDebug>


#define TAB_MAX_COL 5
GameFrameUI::GameFrameUI(QWidget *parent) :
    UIbase(parent),
    ui(new Ui::GameFrameUI)
{
    ui->setupUi(this);
    ui->pushButton_DualMeet->hide();
    ui->pushButton_Fight->hide();
    ui->pushButton_GamesCity->hide();
    ui->pushButton_Guess->hide();
    ui->pushButton_Multiplayer->hide();
    ui->pushButton_SameTable->hide();
    ui->pushButton_Smart->hide();

}

GameFrameUI::~GameFrameUI()
{
    delete ui;
}

void GameFrameUI::updateUserInfo(const go::UserInfo &userInfo)
{
    ui->labelName->setText( tr("Name:")+userInfo.accounts().c_str() );
    ui->labelGold->setText( tr("Gold:\n") + QString::number(userInfo.money()/100,'f',2) );
}

void GameFrameUI::updateRoomInfo(const go::RoomInfo &roomInfo)
{

    auto getBtn = [this](int number)->QPushButton*{
        switch (number) {
        case ROOM_NUM_Fight:
            return ui->pushButton_Fight;
        case ROOM_NUM_Multiplayer:
            return ui->pushButton_Multiplayer;
        case ROOM_NUM_SameTable://牌桌
            return ui->pushButton_SameTable;
        case ROOM_NUM_Guess:
            return ui->pushButton_Guess;
        case ROOM_NUM_GamesCity:
            return ui->pushButton_GamesCity;
        case ROOM_NUM_DualMeet:
            return ui->pushButton_DualMeet;
        case ROOM_NUM_Smart:
            return ui->pushButton_Smart;
        default:
            break;
        }
        return nullptr;
    };
    QPushButton* roomBtn = getBtn(roomInfo.roomnum());
    if(nullptr != roomBtn)
    {
        roomBtn->show();
        roomBtn->setText(roomInfo.roomname().c_str());
        connect(roomBtn, &QPushButton::clicked, [roomInfo](){
            HandleMsg::GetInstance()->ReqEnterRoom(roomInfo.roomnum(), roomInfo.roomkey().c_str());
        });
    }


}





//废弃
/////------------------ 调整TAB----------------------------------
//#include <QPainter>
//#include <QProxyStyle>
//class CustomTabStyle : public QProxyStyle
//{
//public:
//    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
//                           const QSize &size, const QWidget *widget) const
//    {
//        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
//        if (type == QStyle::CT_TabBarTab) {
//            s.transpose();
//            s.rwidth() = 90; // 设置每个tabBar中item的大小
//            s.rheight() = 44;
//        }
//        return s;
//    }

//    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
//    {
//        if (element == CE_TabBarTabLabel) {
//            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
//                QRect allRect = tab->rect;

//                if (tab->state & QStyle::State_Selected) {
//                    painter->save();
//                    painter->setPen(0x89cfff);
//                    painter->setBrush(QBrush(0x89cfff));
//                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
//                    painter->restore();
//                }
//                QTextOption option;
//                option.setAlignment(Qt::AlignCenter);
//                if (tab->state & QStyle::State_Selected) {
//                    painter->setPen(0xf8fcff);
//                }
//                else {
//                    painter->setPen(0x5d5d5d);
//                }

//                painter->drawText(allRect, tab->text, option);
//                return;
//            }
//        }

//        if (element == CE_TabBarTab) {
//            QProxyStyle::drawControl(element, option, painter, widget);
//        }
//    }
//};
/////------------------ 调整TAB----------------------------------END

