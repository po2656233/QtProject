#include "gamemapforfive.h"
//#include "ui_gamemapforfive.h"

GameMapForFive::GameMapForFive(QWidget *parent) :
    UIbase(parent),m_id(0)
//   , ui(new Ui::GameMapForFive)
{
   // ui->setupUi(this);
}

GameMapForFive::~GameMapForFive()
{
 //   delete ui;
}

void GameMapForFive::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    stopBGM();
    emit exitSig(m_id);

}
