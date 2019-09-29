#include "gamemap.h"
#include "base/network/handlemsg.h"
#include <QMessageBox>
#include <QDebug>
GameMap::GameMap(QWidget *parent):
    UIbase(parent),m_id(0)
{
    //如果服务端断开则退出游戏
    //connect(HandleMsg::GetInstance(),SIGNAL(disconnectSig()),this,SLOT(close()));

    //不支持返回
    setRKeyBack(false);
}

GameMap::~GameMap()
{

}

void GameMap::clear()
{
    this->clearSound();
    this->clearFocus();
    this->clearMask();
}

void GameMap::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    stopBGM();
    qDebug()<<"次数 --- >";
    emit exitSig(m_id);
}
