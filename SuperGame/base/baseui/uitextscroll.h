////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): uitextscroll.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-11-03
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef UITEXTSCROLL_H
#define UITEXTSCROLL_H

#include <QLabel>
//跑马灯文字
class UiTextScroll : public QLabel
{
    Q_OBJECT
public:
    enum class Direct{
        Left = 0,
        Right,
        Middle
    };
    explicit UiTextScroll(QWidget *parent = nullptr);
    ~UiTextScroll();
    void setDirect(const Direct& direct);
    void setWords(const QString& text);

    void start();
    void stop();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void onRolling();
private:
    QTimer*  m_pTimer;
    Direct  m_enDirect;
    int m_oneWordWidth;
    int m_pos;
    QString m_strWords;
};

#endif // UITEXTSCROLL_H
