#include "uibetarea.h"
#include "base/common.h"

#include <QTime>
#include <QDropEvent>
#include <QMimeData>
#include <QRandomGenerator>
#include <QLabel>
#include <QBitmap>
#include <QtDebug>
UIBetArea::UIBetArea(QWidget *parent) : GameLabel(parent)
{
    //setAcceptDrops(false);
    m_curChipType = NSType::enChip::ZeroRMB;//测试用

    m_isAllow = false;
}

void UIBetArea::allowClick(bool isAllow)
{
    m_isAllow = isAllow;
}

void UIBetArea::setChipType(NSType::enChip chipType)
{
    m_curChipType = chipType;
}

bool UIBetArea::generateChip(NSType::enChip chipType, const QRect &rect)
{
    QLabel *newPicture = new QLabel(tr("筹码"),this);
    // newPicture->setAttribute(Qt::WA_DeleteOnClose);

    qDebug()<<"当前类型:"<<(int)chipType;

    QPixmap pixmap;
    switch (chipType) {
    case NSType::enChip::OneRMB:
        pixmap.load(":/img/chips/Chip_1.png");
        break;
    case NSType::enChip::TwoRMB:
        pixmap.load(":/img/chips/Chip_2.png");
        break;
    case NSType::enChip::FiveRMB:
        pixmap.load(":/img/chips/Chip_5.png");
        break;
    case NSType::enChip::TwentyRMB:
        pixmap.load(":/img/chips/Chip_25.png");
        break;
    case NSType::enChip::FiftyRMB:
        pixmap.load(":/img/chips/Chip_50.png");
        break;
    case NSType::enChip::HundredRMB:
        pixmap.load(":/img/chips/Chip_100.png");
        break;
    default:
        newPicture->deleteLater();
        return false;
    }
    newPicture->setScaledContents(true);
    newPicture->setGeometry(rect);
    pixmap = pixmap.scaled(rect.size());
    newPicture->setPixmap(pixmap);
    newPicture->resize(pixmap.size());
    newPicture->setMask(pixmap.mask());

    newPicture->show();
    return true;
}

void UIBetArea::clearALL()
{
    //清空所有子空间
    foreach( QObject *obj, this->children())
        if(obj) obj->deleteLater();
}

void UIBetArea::mousePressEvent(QMouseEvent *event)
{
    if(!m_isAllow)return;

    QPoint evPoint = event->pos();
    const QRect& rect = this->geometry();
    int x = evPoint.x();
    int y = evPoint.y();
    int width = rect.width() - 60;
    int height = rect.height() - 60;

    // x的范围 x~x+width   筹码只能出现在 x+1/4width ~ x+width - 1/4width
    // y的范围 y~y+height   筹码只能出现在 y+1/height ~ y+height - 1/height
    //x坐标
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
    x =5+ rand()%(width+1);
    //    //y坐标
    y =5+ rand()%(height+1);
    qDebug()<<"--->>>创建控件 x:"<<x<<" y:"<<y;

    //根据筹码类型随机坐标 显示筹码
    generateChip(m_curChipType, QRect(x,y,30,30));
    QLabel::mousePressEvent(event);
}

//void UIBetArea::dropEvent(QDropEvent *event)
//{
//    const QMimeData* pMime = event->mimeData();
//    if (nullptr == pMime)  return;
//    NSType::enDrag dragType;
//    dragType.setType(NSType::enDrag::Type::Text);
//    qDebug()<<"拖动的类型:"<<dragType.getTypeName();
//    if (pMime->hasFormat( QString(dragType.getTypeName()) )) {//文本控件
//        QByteArray itemData = pMime->data( QString(dragType.getTypeName()) );
//        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

//        QSize size;
//        QString text;
//        QPoint offset;

//        dataStream >> text >> offset >>size;
//        qDebug()<<"生成"<<pMime->text()<<" ->"<<text;

//        QLabel *newPicture = new QLabel(text, this);
//        if(size != QSize(0,0))
//        {
//            newPicture->setFixedSize(size);
//        }
//        newPicture->move(event->pos() - offset);
//        newPicture->show();
//        newPicture->setAttribute(Qt::WA_DeleteOnClose);
//    }
//}
