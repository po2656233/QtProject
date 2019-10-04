#include "chinesechesslogic.h"
#include <QDebug>

#define ROW 10
#define COL 9
#define NUL 0
#define RED 1
#define BLACK 2
//十行九列 // 小写黑 大写红
unsigned char arryChessBoard[ROW][COL]={
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
    m_toJiangX = -1;
    m_toJiangY = -1;
    m_state = enState::BeFine;
    memcpy_s(m_arryChessBoard,sizeof(m_arryChessBoard),arryChessBoard,sizeof(arryChessBoard));
    m_setInfo.clear();
}

bool ChineseChessLogic::CanMove(int x, int y, int toX, int toY)
{

    // x代表着列 y代表行
    if(ROW<=x || COL<=y || (x == toX && y == toY))return false;


    //颜色相同则不符合规则(含0x00)
    if(judgmetColor(x,y) == judgmetColor(toX,toY))
    {
        qDebug()<<"踩到自己人了";
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


bool ChineseChessLogic::JiangJun(int x, int y, bool isSavePos)
{
    m_state = enState::BeFine;
    if(0 == m_arryChessBoard[x][y]) return false;

    int toBX = 0;
    int toBY = 0;
    int toRX = 0;
    int toRY = 0;
    unsigned char piece = 0;
    unsigned char whichSide = judgmetColor(x,y);        // 看下是哪一方.


    // 获取黑方将军的坐标
    getPos('j', toBX, toBY);
    getPos('J', toRX, toRY);
    // qDebug()<<"黑方将军坐标:"<<toX<<" "<<toY;

    //校验当前的棋子是否可以到达将军的位置
    for(int i=0; i<ROW; i++)
    {
        for(int j=0; j<COL; j++)
        {
            if('A'<m_arryChessBoard[i][j] && m_arryChessBoard[i][j]<'S')
            {
                piece = m_arryChessBoard[i][j];
                if(CanMove(i,j, toBX, toBY))
                {//还原坐标
                    m_arryChessBoard[toBX][toBY] = 'j';
                    m_arryChessBoard[i][j] = piece;
                    if(BLACK == whichSide)
                    {
                        Rollback();
                        m_state = enState::BeBad;
                        return false;
                    }
                    else
                    {
                        m_state = enState::BeBJiang;
                        if(isSavePos)
                        {
                            m_toJiangX = i;
                            m_toJiangY = j;
                        }
                        return true;
                    }
                }
            }
            else  if('a'<m_arryChessBoard[i][j] && m_arryChessBoard[i][j]<'s')
            {
                piece = m_arryChessBoard[i][j];
                if(CanMove(i,j, toRX, toRY))
                {//还原坐标
                    m_arryChessBoard[toRX][toRY] = 'J';
                    m_arryChessBoard[i][j] = piece;
                    if(RED == whichSide)
                    {
                        Rollback();
                        m_state = enState::BeBad;
                        return false;
                    }
                    else
                    {
                        m_state = enState::BeRJiang;
                        if(isSavePos)
                        {
                            m_toJiangX = i;
                            m_toJiangY = j;
                        }
                        return true;
                    }
                }
            }
        }
    }
    m_state = enState::BeFine;
    return false;
}

ChineseChessLogic::enState ChineseChessLogic::JudgeWin()
{
    //校验输赢
    //将军的位置
    int jiangX = 0, jiangY = 0;
    int minX = 0, maxX = 0;
    int nCount = 0;


    //四个方位
    //黑方将军是否能移动
    bool bJiang = false;
    enState nSate = m_state;
    if((enState::BeBJiang == nSate && getPos('j',jiangX,jiangY)))
    {
        //黑  移动范围 0 1 2行  3 4 5列
        minX = 0;
        maxX = 3;
        bJiang = true;

    }
    else if(enState::BeRJiang == nSate && getPos('J',jiangX,jiangY))
    {
        //红  移动范围 7 8 9行  3 4 5列
        minX = 7;
        maxX = 10;
        bJiang = true;
    }
    if(bJiang)
    {//校验将军的可行走位置
        if(minX <= jiangX-1)//
        {
            if(CanMove(jiangX, jiangY, jiangX-1, jiangY))
            {
                JiangJun(jiangX-1, jiangY,false);
                if(enState::BeBad == m_state || nSate == m_state) nCount++;
                Rollback();
            }
            else
            {
                nCount++;
            }

        }else
        {
            nCount++;
        }

        if(jiangX+1 < maxX)//
        {
            if(CanMove(jiangX, jiangY, jiangX+1, jiangY))
            {
                JiangJun(jiangX+1,jiangY,false);
                if(enState::BeBad == m_state || nSate == m_state) nCount++;
                Rollback();
            }else
            {
                nCount++;
            }
        }
        else
        {
            nCount++;
        }

        //----------------------//
        if(3 <= jiangY-1)//
        {
            if(CanMove(jiangX, jiangY, jiangX, jiangY-1))
            {
                JiangJun(jiangX,jiangY-1,false);
                if(enState::BeBad == m_state || nSate == m_state) nCount++;
                Rollback();
            }
            else
            {
                nCount++;
            }
        }
        else
        {
            nCount++;
        }

        if(jiangY+1 < 6)//
        {
            if(CanMove(jiangX, jiangY, jiangX, jiangY+1))
            {
                JiangJun(jiangX, jiangY+1,false);
                if(enState::BeBad == m_state || nSate == m_state) nCount++;
                Rollback();
            }
            else
            {
                nCount++;
            }
        }
        else
        {
            nCount++;
        }
        qDebug()<<"---->不能走的步数<----"<<nCount<<" X:"<<m_toJiangX<<" Y:"<<m_toJiangY<<" 棋子"<<(int)m_arryChessBoard[m_toJiangX][m_toJiangY];
        if(4 == nCount)
        {//帅不能动弹，检测其他棋子能否救帅(帮帅挡枪，替帅去死)


            if('M' == m_arryChessBoard[m_toJiangX][m_toJiangY] || 'm' == m_arryChessBoard[m_toJiangX][m_toJiangY])
            {//挡住马脚

              return  nSate;
            }
            else if('P' == m_arryChessBoard[m_toJiangX][m_toJiangY] || 'p' == m_arryChessBoard[m_toJiangX][m_toJiangY]\
                     ||'C' == m_arryChessBoard[m_toJiangX][m_toJiangY] || 'c' == m_arryChessBoard[m_toJiangX][m_toJiangY])
            {//若是炮车 则挡在敌方的前面
                 return  nSate;
            }

            nSate = (enState::BeRJiang == nSate) ? enState::RedWin : enState::BlackWin;
        }
    }
    m_state = nSate;
    return m_state;
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
        if(RED == whichSide)
        {//红
            if(toX<5)
            {
                qDebug()<<"不能过河";
                return false;
            }


            return (x == toX+2 || x == toX-2) &&  (y == toY+2 || y == toY-2);

        }else if(BLACK == whichSide)
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
        if(RED == whichSide)
        {//红  移动范围 7 8 9行  3 4 5列
            if(7<=toX && toX<10 && 3<=toY && toY<6)
            {
                return (x == toX+1 || x == toX-1) &&  (y == toY+1 || y == toY-1);
            }

        }else if(BLACK == whichSide)
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
        if(RED == whichSide)
        {//红  移动范围 7 8 COL行  3 4 5列
            if(7<=toX && toX<ROW && 3<=toY && toY<6)
            {
                return (x == toX && (y == toY+1||y==toY-1)) || (y == toY &&(x == toX+1||x==toX-1));
            }

        }else if(BLACK == whichSide)
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
        if(RED == whichSide)
        {//红方面军 任何时候向前一步都是可以 ->过河之后 同一行时,只能左右走
            return (x == toX+1 && y == toY) || (x < 5 && (toX == x && (y == toY+1 || y == toY-1)));
        }else if(BLACK == whichSide)
        {//黑方面军
            return (x == toX-1 && y == toY) || (4 < x && (toX == x && (y == toY+1 || y == toY-1)));
        }
    }
    return false;
}

bool ChineseChessLogic::getPos(unsigned char piece, int &x, int &y)
{
    for(int i=0; i<ROW; i++)
    {
        for(int j=0; j<COL; j++)
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
    if(0 == m_arryChessBoard[x][y]){//蓝
        return NUL;
    }
    if(0x41 < m_arryChessBoard[x][y] && m_arryChessBoard[x][y] < 0x5A){//红
        return RED;
    }else{//黑
        return BLACK;
    }
}

