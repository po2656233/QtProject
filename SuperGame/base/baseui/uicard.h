////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): poker.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-05-05
/// 功能(@brief) ：   牌值
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef CARD_H
#define CARD_H

#include <QLabel>
//class QLabel;
class UICard : public QLabel
{
    Q_OBJECT
public:
    enum class EnColor{
        //扑克
        FangKuai = 0,
        MeiHua,
        HongTao,
        HeiTao,
        //麻将
        TiaoZi,
        WanZi,
        TongZi,
        FengPai,//风牌
        HuaPai, //花牌
        UnKnown
    };
    UICard(QWidget *parent = nullptr,bool haveCenter = false);
    inline int getCard(){return m_cardValue; }
    void setCard(int card);
    void setCenter(bool isCenter);
    void setBackground(bool isBack, int type = 0);//默认扑克

    // 设置麻将牌
    void setCardMJ(int card);
    //void hasBorder();
signals:
    
public slots:

protected:
    void setColor(EnColor color);
private:
    QLabel* m_value;
    QLabel* m_color;
    QLabel* m_center;
    bool m_haveCenter;
    int m_cardValue;
};

#endif // POKER_H
