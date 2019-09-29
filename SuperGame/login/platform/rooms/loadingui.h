////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): loadingui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-15
/// 功能(@brief) ：   登录进程（也称加载页面，属于场景）
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef LOADINGUI_H
#define LOADINGUI_H

#include "base/baseui/uibase.h"
namespace Ui {
class LoadingUI;
}
QT_BEGIN_NAMESPACE
class QTimer;

class LoadingUI : public UIbase
{
    Q_OBJECT

public:
    explicit LoadingUI(QWidget *parent = 0);
    ~LoadingUI();

    // 加载资源 进度条
    void loadSource(int gameID);

    void setMaxValue(int value);
    void setMinValue(int value);
    void setDuration(int timeout);
signals:
    void sigFinish(int gameID);
protected slots:
    // 进度
    void onProgress();
    // 退出
    void onExit();

private:
    Ui::LoadingUI *ui;
    int             m_gameID;
    int             m_curValue;
    int             m_minValue;
    int             m_maxValue;
    int             m_interval;
    QTimer*         m_timer;
};
QT_END_NAMESPACE
#endif // LOADINGUI_H
