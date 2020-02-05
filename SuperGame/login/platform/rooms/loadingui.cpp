#include "loadingui.h"
#include "ui_loadingui.h"
#include <QTimer>

LoadingUI::LoadingUI(QWidget *parent) :
    UIbase(parent),
    ui(new Ui::LoadingUI)
{
    ui->setupUi(this);

    m_gameID    = 0;
    m_curValue  = 0;
    m_minValue  = 0;
    m_maxValue  = 0;
    m_interval  = 0;
    m_timer = new QTimer(this);
    m_timer->setInterval(100);

    // 不支持退出 和 关闭
    setRKeyBack(false);
    setRKeyExit(false);


    connect(this,SIGNAL(closeSig()), this, SLOT(onExit()));

}

LoadingUI::~LoadingUI()
{
    delete ui;
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_curValue = 0;
}


void LoadingUI::setMaxValue(int value)
{
    m_maxValue = value;
}

void LoadingUI::setMinValue(int value)
{
    m_curValue = value;
}
void LoadingUI::setDuration(int timeout)
{

    if(0 < m_maxValue - m_curValue)
    {
        m_interval = timeout*1000/(m_maxValue - m_curValue);
        m_timer->stop();
        m_timer->setInterval(m_interval);
        m_timer->start();
    }

}

void LoadingUI::loadSource(int gameID,QString name)
{

    ui->progressBar->reset();
    m_curValue = 0;
    m_gameID = gameID;
    ui->label->setText(tr("正在登录\n%1").arg(name));
    ui->progressBar->setMinimum(m_minValue);
    ui->progressBar->setMaximum(m_maxValue);

    disconnect(m_timer, SIGNAL(timeout()), this, SLOT(onProgress()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onProgress()));

    m_timer->start();
}

void LoadingUI::onProgress()
{
    m_curValue++;
    ui->progressBar->setValue(m_curValue);
    if(m_maxValue <= m_curValue)
    {
        m_timer->stop();
        emit this->sigFinish(m_gameID);

    }
}

void LoadingUI::onExit()
{
    m_curValue  = 0;
    m_minValue  = 0;
    m_maxValue  = 0;
    ui->progressBar->reset();
    m_timer->stop();
}
