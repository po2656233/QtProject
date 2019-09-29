////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): uibetarea.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-04
/// 功能(@brief) ：    下注区域
/// 目的(@target)：    可接收筹码
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef UIBETAREA_H
#define UIBETAREA_H

#include "gameproperty.h"
#include "base/common.h"
class UIBetArea : public GameLabel
{
    Q_OBJECT
public:
    explicit UIBetArea(QWidget *parent = nullptr);

    // 允许点击
    void allowClick(bool isAllow);

    // 设置筹码类型
    void setChipType(NSType::enChip chipType);

    // 清空所有筹码
    void clearALL();


protected:
    // 点击事件
    void mousePressEvent(QMouseEvent *event) override;
private:
    // 生成筹码
    bool generateChip(NSType::enChip chipType,const QRect& rect);
private:
    NSType::enChip m_curChipType;
    bool m_isAllow;
};

#endif // UIBETAREA_H
