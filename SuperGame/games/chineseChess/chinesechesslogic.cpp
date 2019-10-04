#include "chinesechesslogic.h"
#include <QDebug>
//十行九列 // 小写黑 大写红
unsigned char arryChessBoard[10][9]={
    'c','m','x','s','j','s','x','m','c',
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,'p',  0,  0,  0,  0,  0,'p',  0,
    'b',  0,'b',  0,'b',  0,'b',  0,'b',
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    'B',  0,'B',  0,'B',  0,'B',  0,'B',
    0,'P',  0,  0,  0,  0,  0,'P',  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,
    'C','M','X','S','J','S','X','M','C',
};
ChineseChessLogic::ChineseChessLogic()
{
    initChessBoard();
}

void ChineseChessLogic::initChessBoard()
{
    m_state = 0x00;
    memcpy_s(m_arryChessBoard,sizeof(m_arryChessBoard),arryChessBoard,sizeof(arryChessBoard));
    m_setInfo.clear();
}

bool ChineseChessLogic::CanMove(int x, int y, int toX, int toY)
{

    // x代表着列 y代表行
    if(10<=x || 9<=y || (x == toX && y == toY))return false;


    //颜色相同则不符合规则(含0x00)
    if(judgmetColor(x,y) == judgmetColor(toX,toY))
    {
        qDebug()<<"踩到自己人了"<<x<<y<<" 自己人的位置:"<<toX<<toY;
        return false;
    }


    //获取棋子类型
    bool isOk = false;
    unsigned char piece = m_arryChessBoard[x][y];
    //qDebug()<<"当前棋子:"<<piece <<" ->"<<m_arryChessBoard[toX][toY];
    switch (piece) {
    case 'C':
    case 'c':
        // qDebug()<<"车的走法";
        isOk = verifyChe(x,y,toX,toY);
        break;
    case 'M':
    case 'm':
        //qDebug()<<"马的走法";
        isOk = verifyMa(x,y,toX,toY);
        break;
    case 'X':
    case 'x':
        //qDebug()<<"象的走法";
        isOk = verifyXiang(x,y,toX,toY);
        break;
    case 'S':
    case 's':
        //qDebug()<<"士的走法";
        isOk = verifyShi(x,y,toX,toY);
        break;
    case 'J':
    case 'j':
        //qDebug()<<"将军的走法";
        isOk = verifyJiang(x,y,toX,toY);
        break;
    case 'P':
    case 'p':
        //qDebug()<<"大炮的走法";
        isOk = verifyPao(x,y,toX,toY);
        break;
    case 'B':
    case 'b':
        //qDebug()<<"兵的走法";
        isOk = verifyBing(x,y,toX,toY);
        break;
    default:
        break;
    }

    if(isOk)
    {
        //qDebug()<<"成功";
        //qDebug()<<x<<"成功"<<y;
        //两位将军不能见面
        int bX = 0, bY = 0;
        int rX = 0, rY = 0;
        getPos('j', bX, bY);
        getPos('J', rX, rY);
        if(bY == rY && ((toX != bX && toY != bY) ||(toX != rX && toY != rY) ) )
        {
            // 两位将军之间是否存在棋子
            bool isCan = false;
            for(int i=bX+1 ; i<rX; i++)
            {
                //假设棋子已经移动
                if(i == x && y == rY) continue;

                if( (i == toX && rY == toY) || 0 < m_arryChessBoard[i][rY])
                {
                    isCan = true;
                    break;
                }
            }
            qDebug()<<"黑方将军位置:"<<bX<<" "<<bY<<" 红方将军位置:"<<rX<<" "<<rY;
            if(!isCan) return false;
        }

        setInfo* info = new setInfo();
        info->x = x;
        info->y = y;
        info->toX = toX;
        info->toY = toY;
        info->prePiece = m_arryChessBoard[x][y];
        info->nowPiece = m_arryChessBoard[toX][toY];

        qDebug()<<x<<"缓存前"<<y;
        m_setInfo.append(info);
        qDebug()<<toX<<"缓存后"<<toY;
        m_arryChessBoard[toX][toY] = m_arryChessBoard[x][y];
        m_arryChessBoard[x][y] = 0;
    }else
    {
        //qDebug()<<"失败"<<x<<" -- "<<y<<piece;
    }


    return isOk;
}

void ChineseChessLogic::Rollback()
{
    int nIndex = m_setInfo.size();
    if( 0 < nIndex )
    {

        setInfo* v = m_setInfo.at(nIndex-1);
        m_arryChessBoard[v->x][v->y] = v->prePiece;
        m_arryChessBoard[v->toX][v->toY] = v->nowPiece;

        qDebug()<<"回滚中..."<<v->x<<" -->" << v->y <<" 棋子-->"<< (int)v->prePiece <<" "<<(int)v->nowPiece;
        m_setInfo.removeAt(nIndex-1);
        delete v;
        v = nullptr;

    }


}


bool ChineseChessLogic::JiangJun(int x, int y)
{
    m_state = 0;
    if(0 == m_arryChessBoard[x][y]) return false;

    int toX = 0;
    int toY = 0;
    unsigned char piece = 0;
    unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方.



    // 获取黑方将军的坐标
    if(false == getPos('j', toX, toY))return false;
    // qDebug()<<"黑方将军坐标:"<<toX<<" "<<toY;

    //校验当前的棋子是否可以到达将军的位置
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {
            if('A'<m_arryChessBoard[i][j] && m_arryChessBoard[i][j]<'S')
            {
                piece = m_arryChessBoard[i][j];
                if(CanMove(i,j, toX, toY))
                {//还原坐标
                    m_arryChessBoard[toX][toY] = 'j';
                    m_arryChessBoard[i][j] = piece;
                    if(0x02 == whichSide)
                    {
                        Rollback();
                        m_state = 0x03;
                        return false;
                    }
                    else
                    {
                        m_state = 0x02;
                        return true;
                    }
                }
            }
        }
    }

    // 获取红方将军的坐标
    if(false == getPos('J', toX, toY))return false;
    //qDebug()<<"红方将军坐标:"<<toX<<" "<<toY;

    //校验当前的棋子是否可以到达将军的位置
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {

            if('a'<m_arryChessBoard[i][j] && m_arryChessBoard[i][j]<'s')
            {
                piece = m_arryChessBoard[i][j];
                if(CanMove(i,j, toX, toY))
                {//还原坐标
                    m_arryChessBoard[toX][toY] = 'J';
                    m_arryChessBoard[i][j] = piece;
                    if(0x01 == whichSide)
                    {
                        Rollback();
                        m_state = 0x03;
                        return false;
                    }
                    else
                    {
                        m_state = 0x01;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int ChineseChessLogic::JudgeWin()
{

    //当将军的时候，进行判断输赢。
    //对面的哪些棋子进行将军。
    //先看一下将军自身是否可以解围，再看一下本方的棋子是否可以解救。

    //将军的位置
    int jiangX = 0, jiangY = 0;
    int minX = 0, maxX = 0;
    int minY = 3, maxY = 6;
    bool bNeedHelp = true;


    //四个方位
    //黑方将军是否能移动
    bool bJiang = false;
    int nSate = m_state;
    if((0x01 == nSate && getPos('j',jiangX,jiangY)))
    {
        //黑  移动范围 0 1 2行  3 4 5列
        minX = 0;
        maxX = 3;
        bJiang = true;

    }
    else if(0x02 == nSate && getPos('J',jiangX,jiangY))
    {
        //红  移动范围 7 8 9行  3 4 5列
        minX = 7;
        maxX = 10;
        bJiang = true;
    }



    if(bJiang)
    {//校验将军的可行走位置
        //炮  马 军 兵 将的是否将军
        for(int i = minX; i<maxX; i++)
        {
            for(int j = minY; j < maxY; j++)
            {
                if(i == jiangX && j == jiangY) continue;
                if(CanMove(jiangX,jiangY,i,j))
                {
                    Rollback();
                    bNeedHelp = false;
                    break;
                }
            }
            if(!bNeedHelp)break;
        }
        if(bNeedHelp)
        {//表示需要解救
            //看对面过来将军的棋子是那个

            //检索本方所有棋子,在移动位置不被将军的情况下，看是否可以阻止被将军。
            //看是否可以吃掉对面的棋子，是否可以挡住马脚 或者 拦住炮
            for(int i = 0; i < 10; i++)
            {
                for(int j = 0; j < 9; j++)
                {
                    if(0 == m_arryChessBoard[i][j] || (i == jiangX && j == jiangY)) continue;
                    if(judgmetColor(i, j) == nSate)
                    {

                    }
                }
            }
            nSate = 0x04;//测试用
        }
        // 当前状态 0x00:不用管 0x01:红 0x02:黑(内部用就不用枚举了)0x03表示不能走，处于被将军状态 0x04表示紅方贏了 0x05表示黑方贏了
    }
    m_state = nSate;
    return nSate;
}



bool ChineseChessLogic::verifyChe(int x, int y, int toX, int toY)
{
    if ('C' == m_arryChessBoard[x][y] || 'c' == m_arryChessBoard[x][y])
    {//确认 棋子
        //车的走法  必须同一行  或者 同一列
        if(x == toX)
        {//若同一行 则校验 直到toY列中是否包含其他棋子
            int start = y;
            int end = toY;
            if(toY < y){
                start = toY;
                end = y;
            }
            for(int i=start+1; i<end; i++){
                if(0 < m_arryChessBoard[x][i])
                {
                    qDebug()<<"遇到障碍 "<<m_arryChessBoard[x][i];
                    return false;
                }
            }
        }else if(y == toY)
        {//若同一列 则校验 直到toX行中是否包含其他棋子
            int start = x;
            int end = toX;
            if(toX < x){
                start = toX;
                end = x;
            }
            for(int i=start+1; i<end; i++){
                if(0 < m_arryChessBoard[i][y])
                {
                    qDebug()<<"遇到障碍 "<<m_arryChessBoard[i][y];
                    return false;
                }
            }
        }else{
            return false;
        }
        return true;
    }
    qDebug()<<"不是车 "<<m_arryChessBoard[x][y];
    return false;
}

bool ChineseChessLogic::verifyMa(int x, int y, int toX, int toY)
{
    if (('M' == m_arryChessBoard[x][y] ||'m' == m_arryChessBoard[x][y] )&& x != toX && y != toY)
    {
        //马的走法  必须不在同一行  且 不在同一列
        if(toX - x == 2 )
        {//竖着走 校验马脚
            if(0 < m_arryChessBoard[x+1][y])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return y == toY-1 || y == toY+1;
        }
        else if(toX - x == -2)
        {
            if(0 < m_arryChessBoard[x-1][y])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return y == toY-1 || y == toY+1;
        }else if(toY - y == 2)
        {//横着走
            if(0 < m_arryChessBoard[x][y+1])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return x == toX-1 || x == toX+1;
        }else if(toY - y == -2)
        {
            if(0 < m_arryChessBoard[x][y-1])
            {
                qDebug()<<"挡住马脚了";
                return false;
            }
            return x == toX-1 || x == toX+1;
        }
    }
    return false;
}

bool ChineseChessLogic::verifyXiang(int x, int y, int toX, int toY)
{
    if (('X' == m_arryChessBoard[x][y] || 'x' == m_arryChessBoard[x][y]) && x != toX && y != toY)
    {//象的走法  不能过河 走田字

        //不能挡住象脚
        if(0 < m_arryChessBoard[(x+toX)/2][(y+toY)/2])
        {
            qDebug()<<"挡住像脚了";
            return false;
        }

        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的象.
        if(0x01 == whichSide)
        {//红
            if(toX<5)
            {
                qDebug()<<"不能过河";
                return false;
            }


            return (x == toX+2 || x == toX-2) &&  (y == toY+2 || y == toY-2);

        }else if(0x02 == whichSide)
        {//黑
            if(4<toX)
            {
                qDebug()<<"不能过河";
                return false;
            }
            return (x == toX+2 || x == toX-2) &&  (y == toY+2 || y == toY-2);
        }
    }
    return false;
}

bool ChineseChessLogic::verifyShi(int x, int y, int toX, int toY)
{
    if (('S' == m_arryChessBoard[x][y] || 's' == m_arryChessBoard[x][y]) && x != toX && y != toY)
    {//仕的走法  必须不在同一行且不在同一列 只能是斜着走
        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的卫仕.
        if(0x01 == whichSide)
        {//红  移动范围 7 8 9行  3 4 5列
            if(7<=toX && toX<10 && 3<=toY && toY<6)
            {
                return (x == toX+1 || x == toX-1) &&  (y == toY+1 || y == toY-1);
            }

        }else if(0x02 == whichSide)
        {//黑  移动范围 0 1 2行  3 4 5列
            if(0<=toX && toX<3 && 3<=toY && toY<6)
            {
                return (x == toX+1 || x == toX-1) &&  (y == toY+1 || y == toY-1);
            }
        }
    }
    return false;
}

bool ChineseChessLogic::verifyJiang(int x, int y, int toX, int toY)
{
    if ('J' == m_arryChessBoard[x][y] || 'j' == m_arryChessBoard[x][y])
    {

        int startX = 0;
        int endX = 0;
        int rX = 0, rY = 0;
        if('j' == m_arryChessBoard[x][y])
        {
            getPos('J', rX, rY);
            startX = toX;
            endX = rX;
        }else{
            getPos('j', rX, rY);
            startX = rX;
            endX = toX;
        }

        //----------------
        if(toY == rY)
        {
            // 两位将军之间是否存在棋子
            bool isCan = false;

            for(int i=startX+1; i<endX; i++)
            {
                if(0 < m_arryChessBoard[i][rY])
                {
                    isCan = true;
                    break;
                }
            }
            if(!isCan) return false;
        }



        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的将军.
        if(0x01 == whichSide)
        {//红  移动范围 7 8 9行  3 4 5列
            if(7<=toX && toX<10 && 3<=toY && toY<6)
            {
                return (x == toX && (y == toY+1||y==toY-1)) || (y == toY &&(x == toX+1||x==toX-1));
            }

        }else if(0x02 == whichSide)
        {//黑  移动范围 0 1 2行  3 4 5列
            if(0<=toX && toX<3 && 3<=toY && toY<6)
            {
                return (x == toX && (y == toY+1||y==toY-1)) || (y == toY &&(x == toX+1||x==toX-1));
            }
        }
    }
    return false;
}

bool ChineseChessLogic::verifyPao(int x, int y, int toX, int toY)
{
    if ('P' == m_arryChessBoard[x][y] || 'p' == m_arryChessBoard[x][y])
    {
        //跑的走法  必须同一行  或者 同一列
        if(x == toX)
        {//若同一行 则校验 直到toY列中是否包含其他棋子
            int start = y;
            int end = toY;
            if(toY < y){
                start = toY;
                end = y;
            }
            if(0 == m_arryChessBoard[toX][toY])
            {//如果目标地 是空,校验是否有障碍物
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[x][i])
                    {
                        qDebug()<<"遇到障碍 "<<m_arryChessBoard[x][i];
                        return false;
                    }
                }
            }else
            {//如果是敌方棋子,校验是否前面有一个障碍
                int nCount = 0;
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[x][i]) nCount++;
                }
                if (1 != nCount)  return false;
            }

        }else if(y == toY)
        {//若同一列 则校验 直到toX行中是否包含其他棋子
            int start = x;
            int end = toX;
            if(toX < x){
                start = toX;
                end = x;
            }

            if(0 == m_arryChessBoard[toX][toY])
            {//如果目标地 是空,校验是否有障碍物
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[i][y])
                    {
                        qDebug()<<"遇到障碍 "<<m_arryChessBoard[i][y];
                        return false;
                    }
                }
            }else
            {//如果是敌方棋子,校验是否前面有一个障碍
                int nCount = 0;
                for(int i=start+1; i<end; i++){
                    if(0 < m_arryChessBoard[i][y]) nCount++;
                }
                if (1 != nCount)  return false;
            }
        }else{
            return false;
        }
        return true;
    }
    return false;
}

bool ChineseChessLogic::verifyBing(int x, int y, int toX, int toY)
{
    if ('B' == m_arryChessBoard[x][y] || 'b' == m_arryChessBoard[x][y])
    {
        // 兵 不能往回走,且只能一步步走. 过河之后 才能左右走.
        unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方的来兵.
        if(0x01 == whichSide)
        {//红方面军 任何时候向前一步都是可以 ->过河之后 同一行时,只能左右走
            return (x == toX+1 && y == toY) || (x < 5 && (toX == x && (y == toY+1 || y == toY-1)));
        }else if(0x02 == whichSide)
        {//黑方面军
            return (x == toX-1 && y == toY) || (4 < x && (toX == x && (y == toY+1 || y == toY-1)));
        }
    }
    return false;
}

bool ChineseChessLogic::getPos(unsigned char piece, int &x, int &y)
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(piece == m_arryChessBoard[i][j])
            {
                x = i;
                y = j;
                return true;
            }

        }
    }
    return false;
}


unsigned char ChineseChessLogic::judgmetColor(int x, int y)
{
    if(0x00 == m_arryChessBoard[x][y])
    {//蓝
        return 0x00;
    }
    if(0x41 < m_arryChessBoard[x][y] && m_arryChessBoard[x][y] < 0x5A)
    {//红
        return 0x01;
    }else
    {//黑
        return 0x02;
    }
}

