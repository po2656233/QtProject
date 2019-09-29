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
    enum class enState{
        BeFine = 0,//
        BeRJiang,
        BeBJiang,
        RedWin,
        BlackWin,
        BeBad // 不能移动
    };

    void initChessBoard();

    //是否可移动
    bool CanMove(int x, int y, int toX, int toY); // 0:表示成功 -1:表示失败 1:表示红方被将军 2:黑方被将军
    //是否将军(x,y是移至的位置)
    bool JiangJun(int x, int y,bool isSavePos = true);//0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)0x03表示不能走，处于被将军状态
    //判断输赢
    enState JudgeWin();               //0x04表示紅方贏了 0x05表示黑方贏了
    //
    inline enState getCode()const{return m_state;}
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

    bool getPos(unsigned char piece, int& x, int& y);//查找棋子的位置
    //判断哪方棋子
    unsigned char judgmetColor(int x, int y);//0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)

private:
    enState m_state;//当前状态 0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)0x03表示不能走，处于被将军状态 0x04表示紅方贏了 0x05表示黑方贏了
    unsigned char m_arryChessBoard[10][9];
    int m_toJiangX;//处于将军状态的棋子的X坐标
    int m_toJiangY;//处于将军状态的棋子的Y坐标
    QList<setInfo*> m_setInfo;
};

#endif // CHINESECHESSLOGIC_H
