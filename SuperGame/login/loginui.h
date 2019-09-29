////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): loginui.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-15
/// 功能(@brief) ：   登录页面(场景)
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef LOGINUI_H
#define LOGINUI_H

#include "base/baseui/uibase.h"
#include "base/global.h"
namespace Ui {
class LoginUI;
class Register;
}
class HandleMsg;
class LoginUI : public UIbase
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = 0);
    ~LoginUI();

private slots:
    void on_pushButtonEnter_clicked();

    void on_pushButton_Home_clicked();

    void on_pushButton_shop_clicked();

    void on_pushButton_forum_clicked();

    void on_pushButton_Newbie_clicked();

    void on_pushButtonRegist_clicked();

    void on_pushButtonForget_clicked();

private:
    Ui::LoginUI     *ui;
    Ui::Register    *uiRegister;
    QDialog*        m_dlgRegister;
};

#endif // LOGINUI_H
