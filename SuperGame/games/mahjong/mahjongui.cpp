#include "mahjongui.h"
#include "ui_mahjong.h"

MahjongUI::MahjongUI(QWidget *parent) :
    GameMap(parent),
    ui(new Ui::Mahjong)
{
    ui->setupUi(this);
}

MahjongUI::~MahjongUI()
{
    delete ui;
}

void MahjongUI::changeState(int state, const std::string &data)
{
    Q_UNUSED(state)
    Q_UNUSED(data)
}

void MahjongUI::enterScene(int scene, const std::string &data)
{
    Q_UNUSED(scene)
    Q_UNUSED(data)
}

bool MahjongUI::gameHandle(int code, const std::string &data)
{
    Q_UNUSED(code)
    Q_UNUSED(data)
    return false;
}

bool MahjongUI::updateInfo(int code, const std::string &data)
{
    Q_UNUSED(code)
    Q_UNUSED(data)
    return false;
}
