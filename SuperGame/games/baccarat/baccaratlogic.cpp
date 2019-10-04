#include "baccaratlogic.h"

#include "base/global.h"
#include <string.h>


BaccaratLogic::BaccaratLogic()
{

}

std::string BaccaratLogic::GetAreaText(unsigned char cbCardData)
{

    Area enArea = static_cast<Area>(cbCardData);
    switch (enArea) {
    case Area::XIAN:
        return "闲家";
    case Area::PING:
        return "平家";
    case Area::ZHUANG:
        return "庄家";
    case Area::XIAN_TIAN:
        return "闲天王";
    case Area::ZHUANG_TIAN:
        return "庄天王";
    case Area::TONG_DUI:
        return "同点平";
    case Area::XIAN_DUI:
        return "闲对子";
    case Area::ZHUANG_DUI:
        return "庄对子";
    default:
        break;
    }
    return "";
}
unsigned char BaccaratLogic::GetArea(const std::string &area)
{
    if (0 == area.compare("闲")){
        return static_cast<unsigned char>(Area::XIAN);
    }else if (0 == area.compare("平")){
         return static_cast<unsigned char>(Area::PING);
    }else if (0 == area.compare("庄")){
         return static_cast<unsigned char>(Area::ZHUANG);
    }else if (0 == area.compare("闲天王")){
         return static_cast<unsigned char>(Area::XIAN_TIAN);
    }else if (0 == area.compare("庄天王")){
         return static_cast<unsigned char>(Area::ZHUANG_TIAN);
    }else if (0 == area.compare("同点平")){
         return static_cast<unsigned char>(Area::TONG_DUI);
    }else if (0 == area.compare("闲对子")){
         return static_cast<unsigned char>(Area::XIAN_DUI);
    }else if (0 == area.compare("庄对子")){
         return static_cast<unsigned char>(Area::ZHUANG_DUI);
    }
    return 0xFF;
}

void BaccaratLogic::CompareCard(const char *cbFirstData, const char *cbSecondData, int &first, int &second)
{

    first = 0, second = 0;
    size_t firstLen = strlen(cbFirstData);
    size_t secondLen = strlen(cbSecondData);
    for(size_t i = 0; i < firstLen; i++)
    {
        if(GetCardValue(cbFirstData[i]) < 10)
        {
            first += GetCardValue(cbFirstData[i]);
        }

    }

    for(size_t i = 0; i < secondLen; i++)
    {
        if(GetCardValue(cbSecondData[i]) < 10)
        {
            second += GetCardValue(cbSecondData[i]);
        }

    }
    first = first%10;
    second = second%10;

}
