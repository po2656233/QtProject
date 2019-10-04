#ifndef TOUBAOUI_H
#define TOUBAOUI_H

#include <QWidget>

namespace Ui {
class TouBaoUI;
}

class TouBaoUI : public QWidget
{
    Q_OBJECT

public:
    explicit TouBaoUI(QWidget *parent = 0);
    ~TouBaoUI();

private:
    Ui::TouBaoUI *ui;
};

#endif // TOUBAOUI_H
