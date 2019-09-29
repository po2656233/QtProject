////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): gamebutton.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-04
/// 功能(@brief) ：    游戏按钮
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef GAMEPROPERTY_H
#define GAMEPROPERTY_H

#include <QPushButton>
#include <QLabel>
class GameWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor WgtColor READ WgtColor WRITE setWgtColor)


public:
    explicit GameWidget(QWidget *parent = nullptr);
public:
    virtual inline QColor WgtColor() const{ return m_color; }
    virtual void setWgtColor(QColor color);


private:
    QColor m_color;
};
class GameLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QColor Color READ LabelColor WRITE setLabelColor)


public:
    explicit GameLabel(QWidget *parent = nullptr);

    //颜色属性
    virtual inline QColor LabelColor() const{ return m_color; }
    virtual void setLabelColor(QColor color);


private:
    QColor m_color;
};

class GameButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor Color READ BtnColor WRITE setBtnColor)


public:
    explicit GameButton(const QString& name,QWidget *parent = nullptr);

public:
    virtual inline QColor BtnColor() const{ return m_color; }
    virtual void setBtnColor(QColor color);
private:
    QColor m_color;
};








#endif // GAMEPROPERTY_H
