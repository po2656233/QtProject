#include "cowcowlogic.h"
#include "base/global.h"
#include <string.h>

std::string CowcowLogic::GetAreaText(unsigned char cbCardData)
{

    CowcowLogic::Area enArea = static_cast<CowcowLogic::Area> (cbCardData);
    switch (enArea) {
    case Area::Tian:
        return "天";
    case Area::Di:
        return "地";
    case Area::Xuan:
        return "玄";
    case Area::Huang:
        return "黄";
    default:
        break;
    }
    return "";
}
CowcowLogic::Area CowcowLogic::GetArea(const std::string &area)
{
    if (0 == area.compare("天") || 0 == area.compare("tian")){
        return Area::Tian;
    }else if (0 == area.compare("地") || 0 == area.compare("di")){
         return Area::Di;
    }else if (0 == area.compare("玄") || 0 == area.compare("xuan")){
         return Area::Xuan;
    }else if (0 == area.compare("黄") || 0 == area.compare("huang")){
         return Area::Huang;
    }
    return Area::Empty;
}

void CowcowLogic::CompareCard(const char *cbFirstData, const char *cbSecondData, int &first, int &second)
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
