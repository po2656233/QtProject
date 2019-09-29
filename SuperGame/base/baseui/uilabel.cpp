#include "uilabel.h"
#include <QDebug>
UILabel::UILabel(QWidget *parent) : QLabel(parent)
{
    m_styleSheet = styleSheet();
    m_bStyle = true;

    connect(this,SIGNAL(clicked()),this, SLOT(onClicked()));
}

void UILabel::setNoStyle(bool haveStyle)
{
    m_bStyle = haveStyle;
}

void UILabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event)

    m_styleSheet = styleSheet();
    if(m_bStyle)
    {
        setStyleSheet("color:#ff6600;background-color: rgb(0, 85, 255);");
    }

}

void UILabel::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    setStyleSheet(m_styleSheet);
}

void UILabel::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit this->clicked();
}

void UILabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    setStyleSheet(m_styleSheet);
}

void UILabel::onClicked()
{
    if (m_bStyle)
    {
        setStyleSheet("color:#ff6600;background-color: rgb(255, 170, 255);");
    }

}
