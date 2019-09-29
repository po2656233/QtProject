#include "zhajinhuaui.h"
#include "ui_zhajinhuaui.h"

ZhaJinHuaUI::ZhaJinHuaUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZhaJinHuaUI)
{
    ui->setupUi(this);
}

ZhaJinHuaUI::~ZhaJinHuaUI()
{
    delete ui;
}
