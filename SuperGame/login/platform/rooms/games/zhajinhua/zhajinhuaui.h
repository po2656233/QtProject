#ifndef ZHAJINHUAUI_H
#define ZHAJINHUAUI_H

#include <QWidget>

namespace Ui {
class ZhaJinHuaUI;
}

class ZhaJinHuaUI : public QWidget
{
    Q_OBJECT

public:
    explicit ZhaJinHuaUI(QWidget *parent = 0);
    ~ZhaJinHuaUI();

private:
    Ui::ZhaJinHuaUI *ui;
};

#endif // ZHAJINHUAUI_H
