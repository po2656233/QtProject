////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): cowcowlogic.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-07-22
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef COWCOWLOGIC_H
#define COWCOWLOGIC_H

#include "base/gameunit.h"
class CowcowLogic:public GameUnit
{
public:
    enum class Area{
        Tian = 2,
        Xuan,
        Di,
        Huang,
        Empty
    };
    //获取区域
    Area GetArea(const std::string &area);
    std::string GetAreaText(unsigned char cbCardData);

    //比牌
    void CompareCard(const char *cbFirstData, const char *cbSecondData, int &first, int &second);
};

#endif // COWCOWLOGIC_H
