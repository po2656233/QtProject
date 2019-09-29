////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): baccaratlogic.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-07-22
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef BaccaratLogic_H
#define BaccaratLogic_H

#include "base/gameunit.h"

//不含构建函数
class BaccaratLogic: public GameUnit
{
public:
    enum class Area{
        XIAN = 0,           //闲家
        PING = 1,           //平家
        ZHUANG = 2,         //庄家
        XIAN_TIAN = 3,      //闲天王
        ZHUANG_TIAN = 4,    //庄天王
        TONG_DUI = 5,       //同点平
        XIAN_DUI = 6,       //闲对子
        ZHUANG_DUI = 7      //庄对子
    };
    BaccaratLogic();
    // 获取区域
    std::string GetAreaText(unsigned char cbCardData);
    unsigned char GetArea(const std::string& area);

    // 比牌大小 引用获取两个数值大小
    void CompareCard(const char* cbFirstData, const char* cbSecondData,int& first, int& second);

};

#endif // BACCARATGAMELOGIC_H
