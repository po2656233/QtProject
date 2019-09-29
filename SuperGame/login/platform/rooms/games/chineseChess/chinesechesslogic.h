////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): chinesechesslogic.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-15
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
///
#ifndef CHINESECHESSLOGIC_H
#define CHINESECHESSLOGIC_H

struct setInfo
{
    int x;
    int y;
    unsigned char prePiece;
    int toX;
    int toY;
    unsigned char nowPiece;
};
#include <QMap>

class ChineseChessLogic
{
public:
    ChineseChessLogic();


    void initChessBoard();

    //是否可移动
    bool CanMove(int x, int y, int toX, int toY); // 0:表示成功 -1:表示失败 1:表示红方被将军 2:黑方被将军
    //是否将军(x,y是移至的位置)
    bool JiangJun(int x, int y);//0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)0x03表示不能走，处于被将军状态
    //判断输赢
    int JudgeWin();               //0x04表示紅方贏了 0x05表示黑方贏了
    // 查找棋子的位置
    bool getPos(unsigned char piece, int& x, int& y);
    //当前状态码
    inline int getCode()const{return m_state;}
    //回滚
    void Rollback();
protected:
    bool verifyChe(int x, int y, int toX, int toY);
    bool verifyMa(int x, int y, int toX, int toY);
    bool verifyXiang(int x, int y, int toX, int toY);
    bool verifyShi(int x, int y, int toX, int toY);
    bool verifyJiang(int x, int y, int toX, int toY);
    bool verifyPao(int x, int y, int toX, int toY);
    bool verifyBing(int x, int y, int toX, int toY);


    //判断哪方棋子
    unsigned char judgmetColor(int x, int y);//0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)

private:
    int m_state;//当前状态 0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)0x03表示不能走，处于被将军状态 0x04表示紅方贏了 0x05表示黑方贏了
    //int m_toJiangX;//处于将军状态的棋子的X坐标
    //int m_toJiangY;//处于将军状态的棋子的Y坐标

    unsigned char m_arryChessBoard[10][9];
    QList<setInfo*> m_setInfo;
};

#endif // CHINESECHESSLOGIC_H
