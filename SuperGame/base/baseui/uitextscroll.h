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
