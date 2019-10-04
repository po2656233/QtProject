#ifndef GAMEMAPFORFIVE_H
#define GAMEMAPFORFIVE_H

#include "base/UI/uibase.h"
//namespace Ui {
//class GameMapForFive;
//}

class GameMapForFive : public UIbase
{
    Q_OBJECT

public:
    explicit GameMapForFive(QWidget *parent = 0);
    ~GameMapForFive();
    inline void setID(int id){m_id = id;}
signals:
    void exitSig(int gameID);

protected:
    void closeEvent(QCloseEvent *event);

private:
    int m_id;
    //Ui::GameMapForFive *ui;

};

#endif // GAMEMAPFORFIVE_H
