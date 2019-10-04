#include "toubaoui.h"
#include "ui_toubaoui.h"

TouBaoUI::TouBaoUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TouBaoUI)
{
    ui->setupUi(this);
}

TouBaoUI::~TouBaoUI()
{
    delete ui;
}
