////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): gamemap.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-04
/// 功能(@brief) ：    子游戏基类
/// 目的(@target)：    多态实现
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "base/baseui/uibase.h"
class GameMap:public UIbase
{
    Q_OBJECT
public:
    using UIbase::UIbase;
    explicit GameMap(QWidget *parent = 0);
    ~GameMap();
    
    ///外部接口
    //获取ID
    inline virtual int getID(){return m_id;}
    inline virtual void setID(int id){m_id = id;}
    
    ///----纯虚函数----
    //场景消息
    virtual void enterScene(int scene, const std::string& data) = 0;
    //框架消息(状态切换)
    virtual void changeState(int state,const std::string& data)=0;
    //游戏处理
    virtual bool gameHandle(int code, const std::string& data) = 0;
    //更新游戏信息
    virtual bool updateInfo(int code, const std::string& data)=0;
    //清空资源
    virtual void clear();

    ///信号
signals:
    void exitSig(int gameID);
    
    ///事件函数
protected:
    void closeEvent(QCloseEvent *event);
    
    ///成员变量
private:
    int m_id; //标识 ID
};

#endif // GAMEMAP_H
