#include "SuperMan.h"
#include <QApplication>
#include <QIcon>
#include "base/baseui/uimeterprocessbar.h"
#include <QToolButton>
#include <QBitmap>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(element);
    QApplication a(argc, argv);

    //测试倒计时
//    UiMeterProcessBar bar;
//    bar.setMaxValue(100);
//    bar.setMinValue(-300);
//    bar.setDuration(30);
//    bar.show();


    SuperMan man;

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/img/wang.jpg")), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);

    return a.exec();
}
