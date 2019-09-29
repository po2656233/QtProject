#include "uimeterprocessbar.h"
#include <QTextCodec>
#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>
UiMeterProcessBar::UiMeterProcessBar(QWidget *parent) : QWidget(parent)
{
    m_sate = 0;
    currentValue = 0;
    maxValue = 0;

    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec);
    resize(167, 167);
    //setAutoFillBackground(false);

    startValueLabel = new QLabel(tr("0%"), this);
    startValueLabel->setFont(QFont("Arial", 11, QFont::Normal));
    startValueLabel->setStyleSheet("color:#898989");
    startValueLabel->setGeometry(35, 140, 25, 20);

    endValueLabel = new QLabel(tr("100%"),this);
    endValueLabel->setFont(QFont("Arial", 11, QFont::Normal));
    endValueLabel->setStyleSheet("color:#898989");
    endValueLabel->setGeometry(97, 140, 50, 20);

    dispayValueLabel = new QLabel(this);
    dispayValueLabel->setStyleSheet("color:#349BDA");

    updateTimer = new QTimer(this);
    updateTimer->setInterval(500);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
}

UiMeterProcessBar::~UiMeterProcessBar()
{
    if (updateTimer->isActive()) {
        updateTimer->stop();
    }
    currentValue = 0;
}

void UiMeterProcessBar::setMaxValue(int value)
{
    maxValue = value;
}

void UiMeterProcessBar::setMinValue(int value)
{
    currentValue = value;
}
void UiMeterProcessBar::setDuration(int timeout)
{

    if(0 < maxValue - currentValue)
    {
        updateTimer->stop();
        int interval = timeout*1000/(maxValue - currentValue);
        updateTimer->setInterval(interval);
        updateTimer->start();
    }

}

void UiMeterProcessBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(1 == m_sate)return;

    if(2 == m_sate)
    {
        dispayValueLabel->setFont(QFont("Arial", 12, QFont::Bold));
        dispayValueLabel->setText(tr("N/A"));
        return;
    }

    QPainter painter(this);
    QColor usedColor(165, 220, 62);
    QColor freeColor("#8B668B"); //215, 215, 215

    painter.translate(width() / 2, height() / 2);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.save();
    painter.rotate(42);
    painter.setPen(QPen(usedColor, 2));
    for (int i = 0; i < currentValue ; ++i) {
        painter.drawLine(0, 70, 0, 80);
        painter.rotate(2.8);
    }
    painter.setPen(QPen(freeColor, 3));
    for (int i = currentValue; i < maxValue ; ++i) {
        painter.drawLine(0, 70, 0, 80);
        painter.rotate(2.8);
    }
    if (currentValue == 0 || currentValue == maxValue) {
        dispayValueLabel->setFont(QFont("Arial", 12, QFont::Bold));
        dispayValueLabel->setText(tr("N/A"));
    }
    else {
        dispayValueLabel->setFont(QFont("Arial", 15, QFont::Bold));
        dispayValueLabel->setText(tr("%1%").arg(currentValue));
    }
    QFontMetrics metrics(dispayValueLabel->font());
    int textwidth = metrics.width(dispayValueLabel->text());
    int textheight = metrics.height();
    dispayValueLabel->setGeometry((width() - textwidth)/2, (height() - textheight)/2 , textwidth, textheight);
    painter.restore();

    painter.translate(-width()/2, -height()/2);
    painter.setBrush(QColor(233, 233, 233));
    painter.setPen(QPen(QColor(233, 233, 233), 15));
    painter.drawEllipse(QRectF((width()/2 - 55), (height()/2 - 55), 110, 110));

    QConicalGradient conicalGradient(width()/2, height()/2, 90);
    conicalGradient.setColorAt(0, QColor(45, 204, 112));
    conicalGradient.setColorAt(1.0, QColor(51, 152, 219));
    painter.setPen(QPen(QBrush(conicalGradient), 30));
    painter.drawEllipse(QRectF((width()/2 - 35), (height()/2 - 35), 70, 70));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(249, 249, 249));
    painter.drawEllipse(QRectF((width()/2 - 30), (height()/2 - 30), 60, 60));
}


void UiMeterProcessBar::mousePressEvent(QMouseEvent *event)
{
    //    bPressFlag = true;
    //    beginDrag = event->pos();
    QWidget::mousePressEvent(event);
}

void UiMeterProcessBar::mouseMoveEvent(QMouseEvent *event)
{
    //    if (bPressFlag) {
    //        QPoint relaPos(QCursor::pos() - beginDrag);
    //        move(relaPos);
    //    }
    QWidget::mouseMoveEvent(event);
}

void UiMeterProcessBar::mouseReleaseEvent(QMouseEvent *event)
{
    //    bPressFlag = false;
    QWidget::mouseReleaseEvent(event);
}

void UiMeterProcessBar::showEvent(QShowEvent *event)
{
    updateTimer->start();
    QWidget::showEvent(event);
}

void UiMeterProcessBar::hideEvent(QHideEvent *event)
{
    if (updateTimer->isActive()) {
        updateTimer->stop();
    }
    //currentValue = 0;
    QWidget::hideEvent(event);
}

void UiMeterProcessBar::onStart()
{
    updateTimer->start();
    m_sate = 0;
}

void UiMeterProcessBar::onPause()
{
    m_sate = 1;
    if (updateTimer->isActive()) {
        updateTimer->stop();
    }
}

void UiMeterProcessBar::onStop()
{
    m_sate = 2;
    if (updateTimer->isActive()) {
        updateTimer->stop();
    }
}

void UiMeterProcessBar::slotUpdateTimer()
{
    if (currentValue >= maxValue){
        if (updateTimer->isActive()) {
            updateTimer->stop();
        }
        update();// 屏蔽时,则可停留在currentValue == maxValue界面，否则N/A表示
        //对外发送终止信号
        if(0!=currentValue && currentValue == maxValue)emit timeoutSig();
        return;
    }
    currentValue++;
    update();
}
