////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): handcard.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-09-07
/// 功能(@brief) ：   展示玩家手牌
/// 目的(@target)：   可定制牌型、牌数目、动态增加牌值
/// 备注(@attention): 默认扑克牌
////////////////////////////////////////////////////
#ifndef UIHANDCARD_H
#define UIHANDCARD_H

#include <QWidget>
#include <QMap>
class UICard;
class UIHandCard : public QWidget
{
    Q_OBJECT
public:
    enum class Type{
        Poker = 0,
        Mahjong
    };
    enum class Direct{
        Left = 0,
        Middle,
        Right,
        OverlayHalf,    // 对半叠加
        Overlay,        //完全盖住
    };
    explicit UIHandCard(QWidget *parent = nullptr);
    inline void setType(const Type& type){m_curType = type;}

    void startPos(int x, int y);
    void setWidth(int width);
    void setHeight(int height);

    void addCard(int value, int space = 0, int index = -1, bool haveCenter = false, Direct direct = Direct::Left);//默认使用当前最大索引值
    void removeCard(int index);
    void clearCards();

    //要出的牌
    QString outCards();

protected:
    //事件过滤 下注筹码可控制
    bool eventFilter(QObject *watched, QEvent *event);
signals:

public slots:

private:
    Type                m_curType;
    int                 m_x;
    int                 m_y;
    int                 m_width;
    int                 m_height;
    QString             m_strOutCards;
    QMap<int,UICard*>   m_mapCards;//牌的位置 和 牌值
};

#endif // HANDCARD_H
