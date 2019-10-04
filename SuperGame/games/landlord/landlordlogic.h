#ifndef LANDLORDLOGIC_H
#define LANDLORDLOGIC_H
#include "base/gameunit.h"
class LandlordLogic:public GameUnit
{
public:
    LandlordLogic();

    // 获取扑克的数值
    unsigned char GetCardValue(unsigned char cbCardData);
    // 获取扑克的花色
    unsigned char GetCardColor(unsigned char cbCardData);
    // 获取扑克的文字
    std::string GetCardText(unsigned char cbCardData);

    // 获取牌型
    unsigned char GetCarType(const std::string& area);
};

#endif // LANDLORDLOGIC_H
