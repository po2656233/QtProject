#include "uihandcard.h"
#include "uicard.h"

#include <QMouseEvent>
#include <QDebug>

UIHandCard::UIHandCard(QWidget *parent) : QWidget(parent)
{

    m_x = 0;
    m_y = 0;
    m_space = 0;

    m_startX = 0;
    m_startY = 0;
    m_state = 0;
    m_width = 30;
    m_height = 45;
    m_direct = Direct::Left;
    m_curType = Type::Poker;
    m_strOutCards.clear();
}

void UIHandCard::startPos(int x, int y)
{
    m_startX = x;
    m_startY = y;
    m_y = m_startY;
}

void UIHandCard::setCardWidth(int width)
{
    m_width = width;
}

void UIHandCard::setCardHeight(int height)
{
    m_height = height;
}

void UIHandCard::setDirect(UIHandCard::Direct direct)
{
    m_direct = direct;
}

void UIHandCard::setSpace(int space)
{
    m_space = space;
}

void UIHandCard::setState(int state)
{
    m_state = state;
}

void UIHandCard::addCard(int value, int index, bool haveCenter)
{
    qDebug()<<m_x<<m_y;
    //删除已有的牌值

    if(index < 0){
        qDebug()<<"----1";
        index = m_mapCards.count();
    }

    if (0<index && m_mapCards.find(index) != m_mapCards.end()){
        removeCard(index);
        qDebug()<<"----2"<<index;
    }

    //牌的类型
    UICard* card = new UICard(this,haveCenter);
    m_mapCards.insert(index, card);

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
        if(m_direct == Direct::Left)
        {
            m_x = upCard->x() + m_width + m_space;
        }
        else if(Direct::OverlayHalf == m_direct)
        {
            m_x =  upCard->x() + m_width/2 + m_space;
        }else{
            m_x = upCard->x() + m_width + m_space;
        }
        qDebug()<<value<<" ->>>>>>>>";
    }else{
        m_x = m_startX;
    }

    qDebug()<<value<<" ->"<<index<<"当前牌值控件"<<m_x<<":"<<m_y<<"  "<<m_width<<":"<<m_height<<endl;
    //牌摆放位置
    card->setGeometry(m_x, m_y, m_width, m_height);
    // 添加到事件过滤器
    card->installEventFilter(this);
    // 未选中
    card->setIndent(m_state);
    card->show();

}

void UIHandCard::removeCard(int index)
{
    m_mapCards.remove(index);
    UICard* card = m_mapCards.take(index);
    delete card;
}

void UIHandCard::clearCards()
{
    m_x = 0;
    m_y = m_startY;

    UICard* card = nullptr;
    while(nullptr != (card = m_mapCards.take(0)))
    {
        card->deleteLater();
    }
    qDeleteAll(m_mapCards.begin(), m_mapCards.end());
    m_mapCards.clear();
    m_strOutCards.clear();
}

void UIHandCard::clearUp()
{
    UICard* card = nullptr;
    m_x = m_startX;
    for (QMap<int,UICard*>::iterator iter = m_mapCards.begin();iter != m_mapCards.end();iter++){
        card = iter.value();
        if(card)
        {
            int indent = card->indent();
            if(Nomarl == indent)
            {//牌插入的位置
                card->setGeometry(m_x, m_y, m_width, m_height);
                card->show();

                if(m_direct == Direct::Left)
                {
                    m_x += m_width + m_space;
                }
                else if(Direct::OverlayHalf == m_direct)
                {
                    m_x += m_width/2 + m_space;
                }else{
                    m_x += m_width + m_space;
                }

            }else if(Up == indent)
            {
                card->setIndent(Out);
                card->hide();
            }

        }
    }
    m_strOutCards.clear();
}

std::string UIHandCard::outCards()
{
    return m_strOutCards.toStdString().c_str();;
}

void UIHandCard::doNot()
{
    UICard* card = nullptr;
    m_x = m_startX;
    for (QMap<int,UICard*>::iterator iter = m_mapCards.begin();iter != m_mapCards.end();iter++){
        card = iter.value();
        if(card && card->indent() < Out)
        {
            card->setIndent(Nomarl);
            card->setGeometry(m_x, m_y, m_width, m_height);
            card->show();

            if(m_direct == Direct::Left)
            {
                m_x += m_width + m_space;
            }
            else if(Direct::OverlayHalf == m_direct)
            {
                m_x += m_width/2 + m_space;
            }else{
                m_x += m_width + m_space;
            }
        }
    }
    m_strOutCards.clear();
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
            if(Nomarl == state)
            {//选中
                m_strOutCards.append(selectCard->getCard());
                selectCard->setGeometry(rect.x(),rect.y()-20,rect.width(),rect.height());
                selectCard->setIndent(Up);
                qDebug()<<"选中了"<<selectCard->getCard();
            }
            else if(Up == state)
            {//取消
                m_strOutCards.remove( selectCard->getCard());
                selectCard->setGeometry(rect.x(),rect.y()+20,rect.width(),rect.height());
                selectCard->setIndent(Nomarl);
                qDebug()<<"取消了"<<selectCard->getCard();
            }

        }
    }
    return QWidget::eventFilter(watched,event);
}


