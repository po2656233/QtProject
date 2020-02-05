////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): superman.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-15
/// 功能(@brief) ：   主游戏框架
/// 目的(@target)：   协调场景之间的切换
/// 备注(@attention): 类似于游戏大厅
////////////////////////////////////////////////////

#ifndef SUPERMAN_H
#define SUPERMAN_H

#include <QObject>
QT_BEGIN_NAMESPACE
class LoginUI;
class LoadingUI;
class MainWindow;
class HomeUI;
class GameFrameUI;
class GameMap;
class HandleMsg;
class BaccaratUI;
class MahjongUI;
class LandlordUI;
class CowcowUI;
class ChineseChessUI;

enum class EnScene{
    Login = 0,
    Loading,
    Home,
    GameFrame,
    GameMap,
    NoScene = 1024
};
class SuperMan : public QObject
{
    Q_OBJECT
public:
    explicit SuperMan(QObject *parent = nullptr);
    ~SuperMan();

    void setSceneBGM(const QString& strMusic);                  // 设置场景音乐(预留)
    void closeAllScene();                                       // 关闭所有场景(预留)
signals:

public slots:
    void onChangeScene(EnScene curScene,EnScene toScene);       // 场景切换
    void onHandleMsg(const QByteArray& data);                   // 消息处理
    void onLoadinGame(int gameID,QString strName);              // 加載游戲
    void onEnterGame(int gameID);                               // 进入子游戏
    void onExitGame(int gameID);                                // 退出子游戏


protected:
    // 内部接口 指令解析
    bool switchCMD(int mainID, int subID);
    bool handleRegister(int subID);
    bool handleLogin(int subID);            // 登录
    bool handleScene(int subID);            // 场景
    bool handleFrame(int subID);            // 框架
    bool handleState(int subID);            // 状态
    bool handlePlayerState(int subID);      // 玩家状态
    bool handleUpdate(int subID);           // 信息更新


private:
    GameMap* currentMap();                  // 进入游戏的哪种场景(//新增子游戏，请在此处添加)


    bool login();                           // 登录（主业务）
    bool gameList();                        // 获取游戏列表
    bool enterRoomResult();                 // 进入房间
    bool enterGameResult();                 // 进入游戏


private:
    // 当前场景
    EnScene m_curScene;

    // 界面展示
    LoginUI*        m_login;
    LoadingUI*      m_loadingUI;
    HomeUI*         m_homeUI;
    GameFrameUI*    m_gameFrameUI;



    // 子游戏界面
    GameMap*        m_gameMapFather;
    BaccaratUI*     m_gameMapBaccarat;
    MahjongUI*      m_gameMapMahjong;
    LandlordUI*     m_gameMapLandlord;
    CowcowUI*       m_gameMapCowcow;
    ChineseChessUI* m_gameMapChineseChess;

    // 消息处理
    HandleMsg*      m_msg;
    std::string     m_data;
    size_t          m_size;

    // 信息更新
    int             m_updateCode; //更新所需子码
    std::string     m_updateInfo;//缓存信息
};
QT_END_NAMESPACE
#endif // SUPERMAN_H
