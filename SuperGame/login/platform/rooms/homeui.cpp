#include "homeui.h"
#include "ui_homeui.h"

// local
#include "base/network/handlemsg.h"
#include "login.pb.h"
#include "base/baseui/gameproperty.h"

// Qt
#include <QDebug>
#include <QMetaProperty>
#include <QGridLayout>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPalette>
#include <QTime>
#include <QKeyEvent>

// 静态成员
int HomeUI::S_GameID = 0;
int HomeUI::S_GameKindID = 0;
int HomeUI::S_GameLevel = 0;

HomeUI::HomeUI(QWidget *parent) :
    UIbase(parent),
    ui(new Ui::HomeUI)
{
    m_animatIsFinish = true;
    ui->setupUi(this);

    m_layoutGames = new QGridLayout(ui->scrollAreaWidgetContents);
}

HomeUI::~HomeUI()
{
    delete ui;
    clearList();
    if(m_AnimationGoup)
    {
        m_AnimationGoup->deleteLater();
    }
    if(m_layoutGames)
    {
        m_layoutGames->deleteLater();
    }
}

void HomeUI::updateGameList(go::GameList &list)
{
    // 清空资源
    clearList();

    // 根据子项信息,生成控件
    int nRow = 0;
    int nCol = 0;

    for(int i = 0; i < list.items_size(); i++)
    {
        const go::GameBaseInfo& info = list.items(i).info();

        // 布局调整
        if(3 <= nCol)
        {
            nRow++;
            nCol = 0;
        }

        QString GameName = info.name().c_str();

        QString level = QString("\n<房间等级:%1>").arg(info.level());
        QString lessscore = QString("\n<最低坐下积分:%1>").arg(info.lessscore());
        QString enterscore = QString("\n<最低进场积分:%1>").arg(info.enterscore());
        GameName = GameName + level + lessscore + enterscore;

        // 添加游戏单元 到 沙盒
        GameButton* gameItemBtn = new GameButton(GameName,this);
        gameItemBtn->setFixedSize(150,100);
        connect(gameItemBtn, &QPushButton::pressed,[=](){

            S_GameKindID = info.kindid();
            S_GameLevel = info.level();
            qDebug()<<"请求进入游戏:"<<list.items(i).id();
            S_GameID = list.items(i).id();
            emit enterSig(S_GameID);
        });

        // 添加控件效果
        QPropertyAnimation* animation = new QPropertyAnimation(gameItemBtn,"Color");
        animation->setDuration(1000);
        animation->setKeyValueAt(0, QColor(34,139,34,0));
        animation->setKeyValueAt(0.5, QColor(34,139,34,155));
        animation->setKeyValueAt(1, QColor(34,139,34,255));
        animation->setLoopCount(1);
        animation->setEasingCurve(QEasingCurve::OutInExpo);
        animation->start(QAbstractAnimation::DeleteWhenStopped);


        m_layoutGames->addWidget(gameItemBtn, nRow, nCol++, Qt::AlignHCenter);
    }
    // 删除动画

}

void HomeUI::clearList()
{
    //清空horizontalLayout布局内的所有元素
    QLayoutItem *child;
    while ((child = m_layoutGames->takeAt(0)) != 0)
    {
        //setParent为NULL，防止删除之后界面不消失
        if(child->widget())
        {
            child->widget()->deleteLater();
        }
        delete child;
        child = nullptr;
    }
}

void HomeUI::on_pushButton_clicked()
{
    //
    /*  声明动画类，并将控制对象 this (this一定是继承自QObject的窗口部件)  以及属性名 "geometry" 传入构造函数  */
    //       QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
    //       /*  设置动画持续时长为 2 秒钟  */
    //       animation->setDuration(2000);
    //       /*  设置动画的起始状态 起始点 (1,2)  起始大小 (3,4)  */
    //       animation->setStartValue(QRect(1, 2, 3, 4));
    //       /*  设置动画的结束状态 结束点 (100,200)  结束大小 (300,400)  */
    //       animation->setEndValue(QRect(100, 200, 300, 400));
    //       /*  设置动画效果  */
    //       animation->setEasingCurve(QEasingCurve::OutInExpo);
    //       /*  开始执行动画 QAbstractAnimation::DeleteWhenStopped 动画结束后进行自清理(效果就好像智能指针里的自动delete animation) */
    //       animation->start(QAbstractAnimation::DeleteWhenStopped);

    // 还没结束时,不让响应动画
    if(!m_animatIsFinish) return;

    // 随机选中
    int size = m_layoutGames->count();
    int selectID = 0;
    QVector<int> vecNums;
    if(1 < size){
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        selectID = rand()%size;
        vecNums.push_back(selectID);

        // 从总数中抽取半数作为随机选中可选项
        int nRandomCount = 0;
        do{
            vecNums.push_back(rand()%size);
        }while(  ++nRandomCount < (size/2) );
        //qDebug()<<"随机选中...控件:"<<selectID<< " 随机个数:"<<nRandomCount<<" 控件总数"<<size;
    }else if(1 == size){
        vecNums.push_back(0);
    }else{
        return;
    }


    // 随机动画
    m_AnimationGoup  = new QSequentialAnimationGroup;
    GameButton *selectGameBtn = nullptr;
    int index = 0;
    for(int i = 0; i < vecNums.size(); i++)
    {
        index = vecNums.at(i);
        QLayoutItem *item = m_layoutGames->itemAt(index);
        GameButton *gameBtn = qobject_cast<GameButton *>(item->widget());
        if(nullptr != gameBtn)
        {
            if(index == selectID)
            {
                selectGameBtn = gameBtn;
                continue;
            }
            // 闪烁动画
            QPropertyAnimation* animation = new QPropertyAnimation(gameBtn,"Color");
            animation->setDuration(100);
            animation->setKeyValueAt(0, QColor(34,139,34,0));
            animation->setKeyValueAt(0.5, QColor(34,139,34,155));
            animation->setKeyValueAt(1, QColor(34,139,34,255));
            animation->setEasingCurve(QEasingCurve::InOutQuad);
            animation->setLoopCount(2);


            // 回归动画
            //QPropertyAnimation* animation = new QPropertyAnimation(gameBtn,"geometry");
            //animation->setDuration(1000);
            //animation->setStartValue(QRect( m_layoutGames->geometry().x(), m_layoutGames->geometry().y(), m_layoutGames->geometry().width()/2,  m_layoutGames->geometry().height()/2));
            //animation->setEndValue( item->geometry() );
            //animation->setEasingCurve(QEasingCurve::InOutQuad);
            //animation->start(QAbstractAnimation::DeleteWhenStopped);

            m_AnimationGoup->addAnimation(animation);
            m_AnimationGoup->addPause(100);
            if(index == selectID)
            {
                selectGameBtn = gameBtn;
            }
        }

    }


    // 选中项处理
    if(nullptr != selectGameBtn)
    {
        qDebug()<<"点击->"<<selectID;
        QPropertyAnimation* animation = new QPropertyAnimation(selectGameBtn,"Color");
        animation->setDuration(200);
        animation->setKeyValueAt(0, QColor(34,139,34,0));
        animation->setKeyValueAt(0.5, QColor(34,139,34,155));
        animation->setKeyValueAt(1, QColor(34,139,34,255));
        animation->setLoopCount(6);
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        m_AnimationGoup->addAnimation(animation);
        m_AnimationGoup->addPause(100);
        connect(animation,SIGNAL(finished()),selectGameBtn,SIGNAL(pressed()));
    }

    // 启动动画
    m_animatIsFinish = false;
    m_AnimationGoup->start(QAbstractAnimation::DeleteWhenStopped);

    // 删除动画
    connect( m_AnimationGoup, &QSequentialAnimationGroup::finished, [this](){
        m_AnimationGoup->deleteLater();
        m_animatIsFinish = true;
    } );

    // 查找qobject的所有属性
    //    const QMetaObject *metaobject =this->metaObject();
    //    int count = metaobject->propertyCount();
    //    for (int i = 0; i < count; ++i) {
    //        QMetaProperty metaproperty = metaobject->property(i);
    //        const char *name = metaproperty.name();
    //        qDebug()<<name;
    //    }

}

void HomeUI::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
        on_pushButton_clicked();
        break;
    default:
        break;
    }
}
