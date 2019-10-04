////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): uibase.cpp
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-04-26
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
///
#include "uibase.h"
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>
UIbase::UIbase(QWidget *parent) : QWidget(parent),m_mediaPlayer(nullptr),m_playlist(nullptr)
{ 
    m_direction = NONE; //这个设置相当重要,不然出现 光标会拖动屏幕一直
    m_isStretch = true;
    m_isShowFull = true;
    m_isCanBack = false;
    m_isCanExit = false;
    m_isLeftPressDown = false;


    //m_rkeyMenu = nullptr;
    m_soundList = nullptr;
    m_soundPlayer = nullptr;

    m_rkeyMenu = new QMenu();
    m_actBack = new QAction(m_rkeyMenu);          // 返回
    m_actExit = new QAction(m_rkeyMenu);          // 退出
    m_actBack->setText("返回");
    m_actExit->setText("退出");

    //默认支持
    setRKeyBack(true);
    setRKeyExit(true);

    // 追踪鼠标
    this->setMouseTracking(true);
    // 无边框
    this->setWindowFlags(Qt::FramelessWindowHint);

}

UIbase::~UIbase()
{
    stopBGM();
    if(m_actExit)
    {
        m_actExit->deleteLater();
    }
    if(m_actBack)
    {
        m_actBack->deleteLater();
    }
    if(m_rkeyMenu)
    {
        m_rkeyMenu->deleteLater();
    }
    if(nullptr != m_mediaPlayer)
    {
        m_mediaPlayer->deleteLater();
    }
    if(nullptr != m_playlist)
    {
        m_playlist->deleteLater();
    }

    if(nullptr != m_soundPlayer)
    {
        m_soundPlayer->deleteLater();
    }
    if(nullptr != m_soundList)
    {
        m_soundList->deleteLater();
    }


}

void UIbase::setBackMusic(const QString &strPath, int volume, QMediaPlaylist::PlaybackMode model)
{
    if(nullptr == m_mediaPlayer)
    {
        m_mediaPlayer = new QMediaPlayer(this);
        m_playlist = new QMediaPlaylist(this);
        m_mediaPlayer->setPlaylist(m_playlist);
    }
    m_playlist->clear();
    m_playlist->setPlaybackMode(model); // 单曲循环
    m_playlist->addMedia(QUrl(strPath));
    m_playlist->setCurrentIndex(1);
    m_mediaPlayer->setVolume(volume);
    m_mediaPlayer->play();

}

void UIbase::setBGMVolume(int volume)
{
    if(nullptr == m_mediaPlayer)
    {
        m_mediaPlayer->setVolume(volume);
    }

}


void UIbase::playBGM()
{
    if(nullptr != m_mediaPlayer)
    {
        m_mediaPlayer->play();
    }
}

void UIbase::pauseBGM()
{
    if(nullptr != m_mediaPlayer)
    {
        m_mediaPlayer->pause();
    }
}
void UIbase::stopBGM()
{
    if(nullptr != m_mediaPlayer)
    {
        m_mediaPlayer->stop();
    }
    if(nullptr != m_playlist)
    {
        m_playlist->clear();
    }
}

void UIbase::setSoundVolume(int volume)
{
    if(nullptr != m_soundPlayer)
    {
        m_soundPlayer->setVolume(volume);
    }
}

void UIbase::addSound(const QString &strPath)
{
    if(nullptr == m_soundPlayer)
    {
        m_soundPlayer = new QMediaPlayer(this);
    }
    if(nullptr == m_soundList)
    {
        m_soundList = new QMediaPlaylist(this);
        m_soundPlayer->setPlaylist(m_soundList);
    }

    m_soundList->addMedia(QUrl(strPath));
    m_soundList->setCurrentIndex(1);

    //m_mediaPlayer->play();
}

void UIbase::playSound(int index, QMediaPlaylist::PlaybackMode model)
{
    if(nullptr != m_soundPlayer)
    {
        if(nullptr != m_soundList && 0<=index)
        {
            m_soundList->setCurrentIndex(index);
            m_soundList->setPlaybackMode(model);
        }
        m_soundPlayer->play();
    }
}

void UIbase::pauseSound()
{
    if(nullptr != m_soundPlayer)
    {
        m_soundPlayer->pause();
    }
}

void UIbase::stopSound()
{
    if(nullptr != m_soundPlayer)
    {
        m_soundPlayer->stop();
    }
}

void UIbase::clearSound()
{

    if(nullptr != m_soundList)
    {
        m_soundList->clear();
    }
}

void UIbase::setStretch(bool isCan)
{
    m_isStretch = isCan;
}

void UIbase::setShowFull(bool isCan)
{
    m_isShowFull = isCan;
}

void UIbase::setRKeyBack(bool isCan)
{
    if(isCan)
    {
        disconnect(m_actBack, SIGNAL(triggered(bool)), this, SLOT(onBack()));
        connect(m_actBack, SIGNAL(triggered(bool)), this, SLOT(onBack()));
        m_rkeyMenu->addAction(m_actBack);
    }else
    {
        disconnect(m_actBack, SIGNAL(triggered(bool)), this, SLOT(onBack()));
        m_rkeyMenu->removeAction(m_actBack);
    }
    m_isCanBack = isCan;
}

void UIbase::setRKeyExit(bool isCan)
{

    if(isCan)
    {
        disconnect(m_actExit, SIGNAL(triggered(bool)), this ,SLOT(close()));
        connect(m_actExit, SIGNAL(triggered(bool)), this ,SLOT(close()));
        m_rkeyMenu->addAction(m_actExit);
    }else
    {
        disconnect(m_actExit, SIGNAL(triggered(bool)), this ,SLOT(close()));
        m_rkeyMenu->removeAction(m_actExit);
    }

    m_isCanExit = isCan;
}

void UIbase::onBack()
{
    this->close();
    emit closeSig();
}


// 实现界面拖拽
void UIbase::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button() || Qt::RightButton == event->button())
    {
        m_isLeftPressDown = true;
        if(m_direction != NONE)
        {
            this->mouseGrabber();
        }
        else
        {
            m_dragPosition = event->globalPos() - pos();
        }

    }
    event->ignore();
    QWidget::mousePressEvent(event);
}

void UIbase::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());


    if (!m_isLeftPressDown) {
        this->judgeRegionSetCursor(gloPoint);
    }else {

        if (m_direction != NONE) {
            QRect rMove(tl, rb);

            switch (m_direction) {
            case LEFT:
                if (rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if (rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if (rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if (rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:
                break;
            }
            this->setGeometry(rMove);
        }
        else {
            move(event->globalPos() - m_dragPosition);
            event->accept();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void UIbase::mouseReleaseEvent(QMouseEvent *event)
{
    m_isLeftPressDown = false;
    m_dragPosition = QPoint(0,0);
    QApplication::restoreOverrideCursor();
    event->ignore();
    QWidget::mouseReleaseEvent(event);
}


// 右键弹窗
void UIbase::contextMenuEvent(QContextMenuEvent *event)
{

    QWidget::contextMenuEvent(event);

    if(m_isCanBack||m_isCanExit)
    {
        m_rkeyMenu->exec(QCursor::pos());
    }


    event->accept();

}

void UIbase::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    if(m_isShowFull && event->button()==Qt::LeftButton)
    {
        windowState()!=Qt::WindowFullScreen?setWindowState(Qt::WindowFullScreen):setWindowState(Qt::WindowNoState);
    }
    event->ignore();
    return QWidget::mouseDoubleClickEvent(event);
}

void UIbase::judgeRegionSetCursor(const QPoint & currentPoint)
{
    if(!m_isStretch)return;

    // 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = currentPoint.x();
    int y = currentPoint.y();
    if(x<0||y<0)return;

    if (tl.x() + m_padding >= x && tl.x() <= x && tl.y() + m_padding >= y && tl.y() <= y) {
        // 左上角
        m_direction = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    }
    else if (x >= rb.x() - m_padding && x <= rb.x() && y >= rb.y() - m_padding && y <= rb.y()) {
        // 右下角
        m_direction = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if (x <= tl.x() + m_padding && x >= tl.x() && y >= rb.y() - m_padding && y <= rb.y()) {
        //左下角
        m_direction = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (x <= rb.x() && x >= rb.x() - m_padding && y >= tl.y() && y <= tl.y() + m_padding) {
        // 右上角
        m_direction = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (x <= tl.x() + m_padding && x >= tl.x()) {
        // 左边
        m_direction = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (x <= rb.x() && x >= rb.x() - m_padding) {
        // 右边
        m_direction = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (y >= tl.y() && y <= tl.y() + m_padding) {
        // 上边
        m_direction = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if (y <= rb.y() && y >= rb.y() - m_padding) {
        // 下边
        m_direction = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else {
        // 默认
        m_direction = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}


