#include "uicard.h"
#include "base/gameunit.h"

#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>



UICard::UICard(QWidget *parent,bool haveCenter) : QLabel(parent),m_haveCenter(haveCenter)
{

    setAutoFillBackground(true);
    m_value = new QLabel(this);
    m_color = new QLabel(this);
    m_center = nullptr;

    m_value->setStyleSheet("border:none;border-image: none;");
    m_color->setStyleSheet("border:none;border-image: none;");

    QGridLayout* layout= new QGridLayout;
    layout->addWidget(m_value,0,0,1,1,Qt::AlignLeft);
    layout->addWidget(m_color,1,0,1,1,Qt::AlignLeft);
    if(m_haveCenter)
    {
        m_center = new QLabel(this);
        layout->addWidget(m_center,2,1,3,3,Qt::AlignLeft);
    }


    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setLayout(layout);
}

void UICard::setColor(UICard::EnColor color)
{
    QPixmap pixmap;
    switch (color) {
    case EnColor::FangKuai:
        pixmap.load(":/img/poker_b/b_bigtag_0.png");
        break;
    case EnColor::MeiHua:
        pixmap.load(":/img/poker_b/b_bigtag_1.png");
        break;
    case EnColor::HongTao:
        pixmap.load(":/img/poker_b/b_bigtag_2.png");
        break;
    case EnColor::HeiTao:
        pixmap.load(":/img/poker_b/b_bigtag_3.png");
        break;
    case EnColor::UnKnown:
        //pixmap.load(":/img/poker_b/b_poker_mask_scale9.png");
        break;
    default:
        break;
    }

    if(m_haveCenter)
    {
        m_center->setPixmap(pixmap);
        m_center->setScaledContents(true);
    }

    m_color->setPixmap(pixmap);
    m_color->setScaledContents(true);
}

void UICard::setCard(int card)
{

    m_cardValue = card;
    int color = card & GameUnit::LOGIC_MASK_COLOR;
    int value = card & GameUnit::LOGIC_MASK_VALUE;
    if(0x00 == card)
    {//等于0时,不适用样式 :/img/poker_b/b_poker_back.png
        setStyleSheet("border-image: url(:/img/poker_b/b_poker_back.png);");
        m_color->setVisible(false);
        m_value->setVisible(false);
        return;
    }
    else
    {
        setStyleSheet("border-image: url(:/img/poker_b/b_bg.png);");
    }

    if(card < 0x01 || 0x3D < card)
    {
        color = 0x40;
        value = 0x10;
    }

    QString strValue = "";
    EnColor enColor;
    switch (color) {
    case 0x00:
        strValue = ":/img/poker_b/b_orange_%1.png";
        enColor = EnColor::FangKuai;
        break;
    case 0x10:
        strValue = ":/img/poker_b/b_black_%1.png";
        enColor = EnColor::MeiHua;
        break;
    case 0x20:
        strValue = ":/img/poker_b/b_red_%1.png";
        enColor = EnColor::HongTao;
        break;
    case 0x30:
        strValue = ":/img/poker_b/b_black_%1.png";
        enColor = EnColor::HeiTao;
        break;
    default:
        enColor = EnColor::UnKnown;
        break;
    }

    setColor(enColor);
    if(0< value && value < 0x0E)
    {
        m_value->setPixmap(QPixmap(strValue.arg( (value & GameUnit::LOGIC_MASK_VALUE)-1)));
    }
    else if(value == 0x0E)
    {
        m_value->setPixmap(QPixmap(":/img/poker_b/b_smalltag_4.png"));
        setColor(EnColor::UnKnown);
    }
    else if(value == 0x0F)
    {
        m_value->setPixmap(QPixmap(":/img/poker_b/b_smalltag_5.png"));
        setColor(EnColor::UnKnown);
    }
    else
    {
        m_value->setPixmap(QPixmap(":/img/poker_b/b_poker_mask_scale9.png"));
    }
    m_value->setScaledContents(true);
    if(m_color->isHidden()) m_color->show();
    if(m_value->isHidden()) m_value->show();
}

void UICard::setCenter(bool isCenter)
{
    if(isCenter)
    {
        if(nullptr == m_center)
        {
            m_center = new QLabel(this);
            QGridLayout* pLayout =  qobject_cast<QGridLayout*>(this->layout()) ;
            pLayout->addWidget(m_center,2,1,3,3,Qt::AlignLeft);
            m_center->setScaledContents(true);
        }

    }
    m_haveCenter = isCenter;
}

void UICard::setBackground(bool isBack, int type)
{
    QString strBJ;
    if(0 == type)
    {
        strBJ =  tr(":/img/poker_b/b_poker_back.png");
    }
    else if(1 == type)
    {
        strBJ =  tr(":/img/majiang/card_duimian.png");
    }

    if(isBack){

        QPixmap bgImg(strBJ);
        if(m_haveCenter)
        {
            m_center->setPixmap(bgImg);
            m_center->setScaledContents(true);
            m_color->hide();
            m_value->hide();
        }
        else
        {
            m_value->setPixmap(bgImg);
            m_value->setScaledContents(true);
            m_value->show();
            m_color->hide();
        }

    }
}

void UICard::setCardMJ(int card)
{
    m_cardValue = card;
    int color = card & GameUnit::LOGIC_MASK_COLOR;
    int value = card & GameUnit::LOGIC_MASK_VALUE;
    //    if(0x00 == card)
    //    {//等于0时,不适用样式
    //        setStyleSheet("");
    //    }
    //    else
    //    {
    //       setStyleSheet("border-image: url(:/img/majiang/card_duimian.png);");
    //    }
    setStyleSheet("border-image: url(:/img/majiang/card_zhengmian.png);");
    if(card < 0x01 || 0x3D < card)
    {
        color = 0x40;
        value = 0x10;
    }

    QString strValue = "";
    //EnColor enColor;
    switch (color) {
    case 0x00:
        strValue = ":/img/majiang/tiao_%1.png.png";
        //enColor = EnColor::TiaoZi;
        break;
    case 0x10:
        strValue = ":/img/majiang/tong_%1.png.png";
        //enColor = EnColor::TongZi;
        break;
    case 0x20:
        strValue = ":/img/majiang/w_%1.png";
        //enColor = EnColor::WanZi;
        break;
    case 0x30:
        strValue = ":/img/majiang/mj_%1.png";
        //enColor = EnColor::FengPai;
        break;
    case 0x40:
        strValue = "";
        //enColor = EnColor::HuaPai;
        break;
    default:
        //enColor = EnColor::UnKnown;
        break;
    }

    m_value->setPixmap(QPixmap(strValue.arg( (value & GameUnit::LOGIC_MASK_VALUE))));
    m_value->setScaledContents(true);
    if(m_color->isHidden()) m_color->show();
    if(m_value->isHidden()) m_value->show();
}

