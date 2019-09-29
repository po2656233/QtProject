////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): UiMeterProcessBar.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-04-26
/// 功能(@brief) ：    计时表
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef UIMETERPROCESSBAR_H
#define UIMETERPROCESSBAR_H

#include <QWidget>
class QLabel;
class UiMeterProcessBar : public QWidget
{
    Q_OBJECT
public:
    explicit UiMeterProcessBar(QWidget *parent = nullptr);
    ~UiMeterProcessBar();
    void setMaxValue(int value);
    void setMinValue(int value);
    void setDuration(int timeout);//设置时长-秒


    //void setSize(int width, int height);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
signals:
    void timeoutSig();
public slots:
    void onStart();
    void onPause();
    void onStop();
private slots:
    void slotUpdateTimer();


private:
    QPoint beginDrag;
    bool bPressFlag;

    int m_sate;//0:开始 1:暂停 2:停止
    int maxValue;
    int currentValue;
    QLabel *startValueLabel;
    QLabel *endValueLabel;
    QLabel *dispayValueLabel;


    QTimer *updateTimer;
};

#endif // UIMETERPROCESSBAR_H
