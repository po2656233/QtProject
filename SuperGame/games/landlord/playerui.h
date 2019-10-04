#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <QWidget>

namespace Ui {
class PlayerUI;
}

class PlayerUI : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerUI(QWidget *parent = 0);
    ~PlayerUI();

private:
    Ui::PlayerUI *ui;
};

#endif // PLAYERUI_H
