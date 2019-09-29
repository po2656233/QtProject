////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): gameunit.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-09-12
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef GAMEUNIT_H
#define GAMEUNIT_H

#include <string>
using namespace std;

//不设构造函数
class GameUnit
{
public:
    enum class Type{
      Color = 0xF0,
      Value = 0x0F
    };
    GameUnit(){}
    virtual ~GameUnit(){}

    static const int LOGIC_MASK_COLOR = 0xF0;
    static const int LOGIC_MASK_VALUE = 0x0F;

    // 获取扑克的数值
    virtual unsigned char GetCardValue(unsigned char cbCardData);
    // 获取扑克的花色
    virtual unsigned char GetCardColor(unsigned char cbCardData);
    // 获取扑克的文字
    virtual std::string GetCardText(unsigned char cbCardData);
};

#endif // GAMEUNIT_H
