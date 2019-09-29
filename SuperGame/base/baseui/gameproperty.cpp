#include "gameproperty.h"

// Qt
#include <QRgb>
#include <QColor>

GameButton::GameButton(const QString &name, QWidget *parent) : QPushButton(parent)
{
    this->setText(name);
}

void GameButton::setBtnColor(QColor color)
{
    m_color = color;
    QString strQSS = QString("color:#000000; background-color: rgba(%1,%2,%3,%4);").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()).arg(m_color.alpha());
    this->setStyleSheet(strQSS);
}


GameWidget::GameWidget(QWidget *parent):QWidget(parent)
{
}

void GameWidget::setWgtColor(QColor color)
{
    m_color = color;
    QString strQSS = QString("color:#000000; background-color: rgba(%1,%2,%3,%4);").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()).arg(m_color.alpha());
    this->setStyleSheet(strQSS);
}

GameLabel::GameLabel(QWidget *parent):QLabel(parent)
{

}

void GameLabel::setLabelColor(QColor color)
{
    m_color = color;
    QString strQSS = QString("color:#000000; background-color: rgba(%1,%2,%3,%4);").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue()).arg(m_color.alpha());
    this->setStyleSheet(strQSS);
}
