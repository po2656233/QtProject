#include "uitextscroll.h"

#include <QTimer>
#include <QPainter>
UiTextScroll::UiTextScroll(QWidget *parent):QLabel(parent)
{
    setMinimumWidth(200);
      setMinimumHeight(40);
    m_pos = 0;
    m_enDirect = Direct::Middle;
    m_strWords = tr("");
    m_oneWordWidth = fontMetrics().width("a");
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(200);
    m_pTimer->start();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(onRolling()));
}

UiTextScroll::~UiTextScroll()
{
    m_pTimer->stop();
    m_pTimer->deleteLater();
}

void UiTextScroll::setDirect(const UiTextScroll::Direct &direct)
{
    m_enDirect = direct;
}

void UiTextScroll::setWords(const QString &text)
{
    stop();
    m_strWords = text;
    start();
}




void UiTextScroll::start()
{
    m_pTimer->start();
}

void UiTextScroll::stop()
{
    m_pos = 10;
    m_pTimer->stop();
}


void UiTextScroll::paintEvent(QPaintEvent *event)
{

    if(m_strWords.isEmpty())return QLabel::paintEvent(event);
    QPainter painter(this);
    //painter.drawText(10, 30, m_strWords.mid(m_pos));
//    QString strWords;
//    switch (m_enDirect) {
//    case Direct::Right:
//        strWords = m_strWords.right(m_pos);
//        break;
//    case Direct::Left:
//        strWords = m_strWords.left(m_pos);
//        break;
//    case Direct::Middle:
//        strWords = m_strWords.mid(m_pos);
//        break;
//    default:
//        return;
//    }

    painter.drawText(width()-m_oneWordWidth*m_pos, 30, m_strWords.left(m_pos));
    return QLabel::paintEvent(event);
}

void UiTextScroll::onRolling()
{

    update();
    m_pos++;
    if (m_pos*m_oneWordWidth > width())
        m_pos = 10;

    return;




//    QString text = this->text();
//    if(text.isEmpty())return;


//    if(m_pos > text.length())
//        m_pos = 10;


//    m_pos+=2;
}
