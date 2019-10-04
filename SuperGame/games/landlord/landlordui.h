#ifndef LANDLORDUI_H
#define LANDLORDUI_H
#include "base/baseui/gamemap.h"


namespace Ui {
class LandlordUI;

}
class QHBoxLayout;
class LandlordUI : public GameMap
{
    Q_OBJECT

public:
    explicit LandlordUI(QWidget *parent = 0);
    ~LandlordUI();

    void enterScene(int scene,const std::string& data);
    void changeState(int state,const std::string& data);
    bool gameHandle(int code,const std::string& data);
    bool updateInfo(int code, const std::string& data);
protected:
    void readyState();
    void enterState();
    void startState();
    void overState();

    // 拖拽界面
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event) ;

private slots:

    void on_pushButton_Hint_clicked();

    void on_pushButton_NoOut_clicked();

    void on_pushButton_Out_clicked();

private:
    void initUI();
    

    void resetTabel();
    void faPai();

    Ui::LandlordUI *ui;
    int m_curState;
    qint64 m_userID;
    qint64 m_playerGold;
};

#endif // LANDLORDUI_H
