#include "gameunit.h"


//获取数值
unsigned char GameUnit::GetCardValue(unsigned char cbCardData)  {
    return cbCardData & static_cast<unsigned char>(Type::Value);
}

//获取花色
unsigned char GameUnit::GetCardColor(unsigned char cbCardData)  {
    return cbCardData & static_cast<unsigned char>(Type::Color);
}

// 文字
std::string GameUnit::GetCardText(unsigned char cbCardData) {
    unsigned char color = GetCardColor(cbCardData);
    unsigned char value = GetCardValue(cbCardData);
    std::string strTxt = "";
    switch (color)
    {
    case 0x00:
        strTxt = "♦";
        break;
    case 0x10:
        strTxt = "♣";
        break;
    case 0x20:
        strTxt = "♥";
        break;
    case 0x30:
        strTxt = "♠";
        break;
    }

    switch (value )
    {
    case 0x00:
        return "";
    case 0x01:
        strTxt += "1";
        break;
    case 0x02:
        strTxt += "2";
        break;
    case 0x03:
        strTxt += "3";
        break;
    case 0x04:
        strTxt += "4";
        break;
    case 0x05:
        strTxt += "5";
        break;
    case 0x06:
        strTxt += "6";
        break;
    case 0x07:
        strTxt += "7";
        break;
    case 0x08:
        strTxt += "8";
        break;
    case 0x09:
        strTxt += "9";
        break;
    case 0x0A:
        strTxt += "10";
        break;
    case 0x0B:
        strTxt += "J";
        break;
    case 0x0C:
        strTxt += "Q";
        break;
    case 0x0D:
        strTxt += "K";
        break;
    case 0x0E:
        strTxt += "小王";
        break;
    case 0x0F:
        strTxt += "大王";
        break;
    }
    return strTxt;
}
