#include "playerui.h"
#include "ui_playerui.h"

PlayerUI::PlayerUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerUI)
{
    ui->setupUi(this);
}

PlayerUI::~PlayerUI()
{
    delete ui;
}
