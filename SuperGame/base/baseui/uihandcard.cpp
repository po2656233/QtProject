#include "uihandcard.h"
#include "uicard.h"

#include <QMouseEvent>
#include <QDebug>

UIHandCard::UIHandCard(QWidget *parent) : QWidget(parent)
{
    m_x = 0;
    m_y = 0;
    m_width = 20;
    m_height = 35;
}

void UIHandCard::startPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

void UIHandCard::setWidth(int width)
{
    m_width = width;
}

void UIHandCard::setHeight(int height)
{
    m_height = height;
}

void UIHandCard::addCard(int value, int space, int index, bool haveCenter, UIHandCard::Direct direct)
{
    //删除已有的牌值
    if(-1 == index){
        index = m_mapCards.count();
    }else if (m_mapCards.find(index) != m_mapCards.end()){
        removeCard(index);
    }

    //牌的类型
    UICard* card = new UICard(this,haveCenter);
    if(m_curType == Type::Poker){
        card->setCard(value);
    }else if(m_curType == Type::Mahjong){
        card->setCardMJ(value);
    }

    //牌的位置
    UICard* upCard = nullptr;
    QMap<int,UICard*>::iterator upIter =  m_mapCards.find(index - 1);
    if (upIter != m_mapCards.end()){
        upCard = upIter.value();
    }

    if(nullptr != upCard)
    {
        //牌插入的位置
        if(direct == Direct::Left)
        {
            m_x = upCard->x()+m_width;
        }
        else if(Direct::OverlayHalf == direct)
        {
            m_x = upCard->x()+m_width/2;
        }
    }

    card->setGeometry(m_x, m_y, m_width, m_height);
    card->show();

    // 添加到事件过滤器
    card->installEventFilter(this);
    // 未选中
    card->setIndent(0);
    m_mapCards.insert(index, card);
}

void UIHandCard::removeCard(int index)
{
    m_mapCards.remove(index);
    UICard* card = m_mapCards.take(index);
    card->deleteLater();
}

void UIHandCard::clearCards()
{
    qDeleteAll(m_mapCards.begin(), m_mapCards.end());
    m_mapCards.clear();
    m_strOutCards.clear();
}

QString UIHandCard::outCards()
{
    return m_strOutCards;
}

bool UIHandCard::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(event);
    if(mouseEvent->type() == mouseEvent->MouseButtonPress ){
        UICard* selectCard = qobject_cast<UICard*>(watched);
        if(selectCard)
        {
            const QRect& rect = selectCard->geometry();
            int state = selectCard->indent();
            if(0 == state)
            {//选中
                m_strOutCards += tr("%1").arg(selectCard->getCard());
                selectCard->setGeometry(rect.x(),rect.y()-20,rect.width(),rect.height());
                selectCard->setIndent(1);
            }
            else if(1 == state)
            {//取消
                m_strOutCards.remove( tr("%1").arg(selectCard->getCard()));
                selectCard->setGeometry(rect.x(),rect.y()+20,rect.width(),rect.height());
                selectCard->setIndent(0);
            }
            qDebug()<<"选中了";
        }
    }
    return QWidget::eventFilter(watched,event);
}


