//local
#include "superman.h"
#include "login/loginui.h"
#include "login/platform/rooms/loadingui.h"
#include "login/platform/rooms/homeui.h"
#include "login/platform/gameframeui.h"
#include "games/baccarat/baccaratui.h"
#include "games/mahjong/mahjongui.h"
#include "games/landlord/landlordui.h"
#include "games/cowcow/cowcowui.h"
#include "games/chineseChess/chinesechessui.h"

#include "base/network/handlemsg.h"
#include "base/common.h"


// proto
#include <login.pb.h>
//#include <baccarat.pb.h>

// Qt
#include <QDebug>
#include <QMessageBox>

using namespace NSNetwork;
SuperMan::SuperMan(QObject *parent) : QObject(parent),m_curScene(EnScene::NoScene)
{


    m_data = "";
    m_updateCode = 0;
    m_updateInfo = "";

    // 登录界面
    m_login = new LoginUI();

    // 加载页
    m_loadingUI = new LoadingUI();


    // 游戏主页
    m_homeUI = new HomeUI();

    // 游戏平台
    m_gameFrameUI = new GameFrameUI();


    // 子游戏
    m_gameMapFather = nullptr;            // 所有游戏统一接口
    m_gameMapBaccarat = new BaccaratUI();
    m_gameMapMahjong = new MahjongUI();
    m_gameMapLandlord = new LandlordUI();
    m_gameMapCowcow = new CowcowUI();
    m_gameMapChineseChess = new ChineseChessUI();


    // 默認登陸頁
    onChangeScene(EnScene::NoScene, EnScene::Login);

    // 加载游戏
    connect(m_homeUI,SIGNAL(enterSig(int)), this, SLOT(onLoadinGame(int)));

    // 進入游戏
    connect(m_loadingUI,SIGNAL(sigFinish(int)), this, SLOT(onEnterGame(int)));

    // 消息处理
    m_msg = HandleMsg::GetInstance();
    connect(m_msg,SIGNAL(recvSig(QByteArray)),this,SLOT(onHandleMsg(QByteArray)));

    // 界面返回
    connect(m_homeUI, &UIbase::closeSig, [this](){
        onChangeScene(EnScene::Home,EnScene::GameFrame);
    });
    connect(m_loadingUI, &UIbase::closeSig, [this](){
        onChangeScene(EnScene::Loading, EnScene::Home);
    });
    connect(m_gameFrameUI, &UIbase::closeSig, [this](){
        onChangeScene(EnScene::GameFrame, EnScene::Login);
    });


}

SuperMan::~SuperMan()
{
    if(nullptr != m_gameMapBaccarat)
    {
        m_gameMapBaccarat->deleteLater();
    }
    if(nullptr != m_gameMapMahjong)
    {
        m_gameMapMahjong->deleteLater();
    }
    if(nullptr != m_gameMapLandlord)
    {
        m_gameMapLandlord->deleteLater();
    }
    if(nullptr != m_gameMapCowcow)
    {
        m_gameMapCowcow->deleteLater();
    }
    if(nullptr != m_gameMapChineseChess)
    {
        m_gameMapChineseChess->deleteLater();
    }


    if(nullptr != m_gameFrameUI)
    {
        m_gameFrameUI->deleteLater();
    }
    if(nullptr != m_homeUI)
    {
        m_homeUI->deleteLater();
    }
    if(nullptr != m_loadingUI)
    {
        m_loadingUI->deleteLater();
    }
    if(nullptr != m_login)
    {
        m_login->deleteLater();
    }

}

// 备用
void SuperMan::setSceneBGM(const QString &strMusic)
{
    auto getBaseUI = [this]()->UIbase*{
            switch (m_curScene) {
            case EnScene::Login:
            return m_login;
            case EnScene::Loading:
            return m_loadingUI;
            case EnScene::Home:
            return m_homeUI;
            case EnScene::GameFrame:
            return m_gameFrameUI;
            case EnScene::GameMap:
            return m_gameMapBaccarat;
            case EnScene::NoScene:
            break;
            default:
            break;}
            return nullptr;};
    UIbase* ui = getBaseUI();
    ui->setBackMusic(strMusic);

}

void SuperMan::closeAllScene()
{
    m_gameMapBaccarat->stopBGM();
    m_login->close();

    m_loadingUI->stopBGM();
    m_loadingUI->close();

    m_homeUI->stopBGM();
    m_homeUI->close();

    m_gameFrameUI->stopBGM();
    m_gameFrameUI->close();

    m_gameMapBaccarat->stopBGM();
    m_gameMapBaccarat->close();

    m_gameMapCowcow->stopBGM();
    m_gameMapCowcow->close();

    m_gameMapLandlord->stopBGM();
    m_gameMapLandlord->close();

    m_gameMapMahjong->stopBGM();
    m_gameMapMahjong->close();

    m_gameMapChineseChess->stopBGM();
    m_gameMapChineseChess->close();

}

void SuperMan::onHandleMsg(const QByteArray& data)
{
    int nCout = 0;
    QByteArray cmdInfo = data;
    do{
        m_data = m_msg->parseData(cmdInfo.data(), cmdInfo.size(), m_size);
        if(m_data.empty() && 0 == m_msg->GetMID())return;
        if(switchCMD(m_msg->GetMID(),m_msg->GetSID()))
        {
            qDebug()<<"正在处理"<<m_msg->GetMID()<<"  "<<m_msg->GetSID()<<" size:"<<m_size << data;
        }
        else
        {
            qDebug()<<"待处理"<<m_msg->GetMID()<<"  "<<m_msg->GetSID() << data;
        }

        cmdInfo.clear();
        std::string unpackData = m_msg->GetUnpack();
        cmdInfo.setRawData(unpackData.c_str(),unpackData.size());
    }while( !cmdInfo.isEmpty() && nCout++ < 20 );
}

void SuperMan::onLoadinGame(int gameID)
{
    // 界面切换->加载游戏
    onChangeScene(EnScene::Home, EnScene::Loading);

    // 加载游戏（暫定50）
    m_loadingUI->setMaxValue(10);
    m_loadingUI->loadSource(gameID);

    m_loadingUI->setWindowState( m_homeUI->windowState() == Qt::WindowFullScreen ? Qt::WindowFullScreen : Qt::WindowNoState);
}

void SuperMan::onEnterGame(int gameID)
{
    // 请求进入游戏
    m_msg->ReqEnterGame(gameID);
}

void SuperMan::onExitGame(int gameID)
{
    qDebug()<<"退出当前游戏---GameID->"<<gameID;
    m_msg->ReqExitGame(gameID);
    onChangeScene(EnScene::NoScene,EnScene::GameFrame);
}


void SuperMan::onChangeScene(EnScene curScene, EnScene toScene)
{

    switch (curScene) {
    case EnScene::Login:
        m_login->close();
        m_login->stopBGM();
        break;
    case EnScene::Loading:
        m_loadingUI->close();
        m_loadingUI->stopBGM();
        break;
    case EnScene::Home:
        m_homeUI->close();
        m_homeUI->stopBGM();
        break;
    case EnScene::GameFrame:
        m_gameFrameUI->close();
        m_gameFrameUI->stopBGM();
        break;
    case EnScene::GameMap:
        m_gameMapFather->clear();//
        m_gameMapFather->close();
        m_gameMapFather->stopBGM();
        m_gameMapFather = nullptr; // 这里已经关闭游戏 父类接口了
        break;
    case EnScene::NoScene:
        break;
    default:
        break;
    }

    // 当前场景
    m_curScene = toScene;
    // 展示当前场景(默认场景音乐: 可优化到配置文件)
    switch (toScene) {
    case EnScene::Login:
        m_login->show();
        m_login->setBackMusic("qrc:/sound/home/login_dudu.wav");
        qDebug()<<"成功获取主页信息";
        break;
    case EnScene::Loading:
        m_loadingUI->show();
        m_loadingUI->setBackMusic("qrc:/sound/home/roomEnter.mp3");
        qDebug()<<"成功 抵达数据加载页";
        break;
    case EnScene::Home:
        m_homeUI->show();
        m_homeUI->setBackMusic("qrc:/sound/home/jinxingqu1.mp3");
        qDebug()<<"成功 抵达 游戏主页";
        break;
    case EnScene::GameFrame:
        m_gameFrameUI->show();
        m_gameFrameUI->setBackMusic("qrc:/sound/chinesechess/floor.wav");
        qDebug()<<"成功 抵达 平台主页";
        break;
    case EnScene::GameMap:         // 预留
        m_gameMapFather->show();   // [注意]子游戏
        m_gameMapFather->setBackMusic("qrc:/sound/home/noonlylove.wav");
        qDebug()<<"成功 进入 游戏";
        break;
    case EnScene::NoScene:
        break;
    default:
        break;
    }


}

GameMap *SuperMan::currentMap()
{
    // 游戏主页生成ID
    switch (HomeUI::S_GameKindID) {
    case GAME_COWCOW:
        m_gameMapCowcow->setBackMusic("qrc:/sound/home/noonlylove.wav");
        return m_gameMapCowcow;
    case GAME_HKFIVE:

        break;
    case GAME_ZHAJINHUA:

        break;
    case GAME_BACCARAT:
        m_gameMapBaccarat->setBackMusic("qrc:/sound/home/sidacaizi.mp3");
        return m_gameMapBaccarat;
    case GAME_TOUBAO:

        break;
    case GAME_LANDLORD:
        m_gameMapLandlord->setBackMusic("qrc:/sound/landlord/MusicEx_Normal2.ogg");
        return m_gameMapLandlord;
    case GAME_MAHJONG:
        m_gameMapMahjong->setBackMusic("qrc:/sound/home/noonlylove.mp3");
        return m_gameMapMahjong;
    case GAME_ChinessChess:
        m_gameMapChineseChess->setBackMusic("qrc:/sound/chinesechess/floor.wav",30);
        return m_gameMapChineseChess;
    default:
        break;
    }
    return nullptr;
}
bool SuperMan::login()
{
    go::MasterInfo info;
    if(info.ParseFromArray(m_data.c_str(), m_size))
    {
        // 界面切换->平台
        onChangeScene(EnScene::Login,EnScene::GameFrame);

        // 更新界面信息
        m_gameFrameUI->updateUserInfo(info.userinfo());
        for(int i = 0; i < info.roomsinfo_size(); i++)
        {
            m_gameFrameUI->updateRoomInfo( info.roomsinfo(i) );
        }

        return true;
    }
    return false;
}

bool SuperMan::gameList()
{
    go::GameList list;
    if(list.ParseFromArray(m_data.c_str(),m_data.length()))
    {
        // 界面切换->主页
        onChangeScene(EnScene::GameFrame, EnScene::Home);

        // 更新界面信息
        m_homeUI->updateGameList(list);
        qDebug()<<"成功登录房间->";
        return true;
    }
    return false;
}

bool SuperMan::enterRoomResult()
{
    qDebug()<<"进入房间...";
    return false;
}

bool SuperMan::enterGameResult()
{
    go::ResResult result;
    if( result.ParseFromArray(m_data.c_str(), m_data.length())  )
    {
        if(SUCCESS == result.state())
        {
            qDebug()<<"---->进入游戏...";
        }
        else
        {
            qDebug()<<result.hints().c_str();
            QMessageBox::warning(m_loadingUI, tr("服务器反馈"), tr(result.hints().c_str()));
            onChangeScene(EnScene::Loading, EnScene::Home);
        }
        return true;
    }
    qDebug()<<"未知错误...";
    return false;
}




bool SuperMan::switchCMD(int mainID, int subID)
{
    switch (mainID)
    {
    case MainRegister:                  // 注册消息
        return handleRegister(subID);
    case MainLogin:                     // 登录消息
        return handleLogin(subID);
    case MainGameSence:                 // 场景消息
        return handleScene(subID);
    case MainGameFrame:                 // 框架消息
        return handleFrame(subID);
    case MainGameState:                 // 游戏状态
        return handleState(subID);
    case MainPlayerState:               // 玩家状态
        return handlePlayerState(subID);
    case MainGameUpdate:                // 更新标识
        return handleUpdate(subID);
    default:
        break;
    }
    return false;

}

bool SuperMan::handleRegister(int subID)
{
    switch (subID) {
    case SubRegister:

        break;
    default:
        break;
    }
    return false;

}

bool SuperMan::handleLogin(int subID)
{
    // 默认登录界面
    // 场景切换->登录界面
    //onChangeScene(m_curScene, EnScene::Login);

    switch (subID) {
    case SubLoginResult:

        break;
    case SubMasterInfo:
        return login();
    case SubGameList:
        return gameList();
    case SubEnterRoomResult:
        return enterRoomResult();
    case SubEnterGameResult:
        return enterGameResult();
    default:
        break;
    }
    return false;

}

bool SuperMan::handleScene(int subID)
{
    // 获取当前游戏
    m_gameMapFather = currentMap();
    if(nullptr != m_gameMapFather)
    {
        m_gameMapFather->setWindowState( m_homeUI->windowState() == Qt::WindowFullScreen ? Qt::WindowFullScreen : Qt::WindowNoState);

        // 界面切换->游戏
        onChangeScene(EnScene::Loading, EnScene::GameMap);
        currentMap();
        m_gameMapFather->setID(HomeUI::S_GameID);
        m_gameMapFather->enterScene(subID, m_data);

        // 游戏返回首页
        qDebug()<<"进入场景 !!!";
        disconnect(m_gameMapFather, SIGNAL(exitSig(int)), this, SLOT(onExitGame(int)));
        connect(m_gameMapFather, SIGNAL(exitSig(int)), this, SLOT(onExitGame(int)));

        if(!m_updateInfo.empty())
        {
            handleUpdate(m_updateCode);
            m_updateInfo.clear();
            m_updateCode = 0;
        }
        return true;
    }
    else
    {
        qDebug()<<"无效的游戏场景，进入场景失败 !!!";
    }


    return false;
}

bool SuperMan::handleFrame(int subID)
{
    qDebug()<<"handleFrame"<<subID;
    if(nullptr != m_gameMapFather)
    {
        // 游戏处理
        return m_gameMapFather->gameHandle(subID,m_data);
    }
    return false;
}

bool SuperMan::handleState(int subID)
{
    qDebug()<<"handleState"<<subID;

    if(nullptr != m_gameMapFather)
    {
        // 状态切换
        m_gameMapFather->changeState(subID, m_data);
        return true;
    }
    qDebug()<<"mei有"<<subID;
    return false;

}

bool SuperMan::handlePlayerState(int subID)
{

    qDebug()<<"handlePlayerState"<<subID;
    return false;

}

bool SuperMan::handleUpdate(int subID)
{
    qDebug()<<"handleUpdate"<<subID;

    if(nullptr != m_gameMapFather)
    {
        // 状态切换
        return m_gameMapFather->updateInfo(subID, m_data);
    }
    m_updateCode = subID;
    m_updateInfo = m_data;
    qDebug()<<"无效 的游戏"<<subID;
    return false;

}


