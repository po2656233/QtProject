////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): global.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-04
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
/////////////////->ROOM<-/////////////////////////////////////
//房间类型
#define ROOM_TYPE_Fight	1
#define ROOM_TYPE_Multiplayer 2
#define ROOM_TYPE_SameTable   3
#define ROOM_TYPE_Guess	4
#define ROOM_TYPE_GamesCity  5
#define ROOM_TYPE_DualMeet   6
#define ROOM_TYPE_Sport 	7
#define ROOM_TYPE_Smart	8

//房间号
#define ROOM_NUM_Multiplayer	101	//
#define ROOM_NUM_Fight      102	// 对战	6	20000
#define ROOM_NUM_DualMeet	103	// 双人对战	7	20000
#define ROOM_NUM_SameTable	104	// 同桌	4,5,6	20000
#define ROOM_NUM_GamesCity	105	// 电玩城	3	20000
#define ROOM_NUM_Guess      106	// 竞猜	4	20000
#define ROOM_NUM_Smart      107	// 益智	4	20000

///////////////////->GAME<-////////////////////////////////
//KINDID
#define GAME_COWCOW             1001
#define GAME_HKFIVE             1002
#define GAME_ZHAJINHUA          2003
#define GAME_BACCARAT           2001
#define GAME_TOUBAO             2002
#define GAME_LANDLORD           3001
#define GAME_MAHJONG            3002
#define GAME_ChinessChess       8002



//MainID 200+ || SubID 100+
//注册
#define MainRegister            200 //-> 注册
#define SubRegister             101 //-> 注册

//登录
#define MainLogin               201 //-> 登录
#define SubLoginResult          101 //-> 登录结果
#define SubMasterInfo           102 //-> 主页信息
#define SubGameList             103 //-> 游戏列表
#define SubEnterRoomResult      104 //进入房间结果
#define SubEnterGameResult      105 //进入游戏结果

//场景
#define MainGameSence           300 //游戏场景
#define SubGameSenceStart       101 //起始
#define SubGameSencePlaying     102 //游戏中
#define SubGameSenceOver        103 //游戏结束
#define SubGameSenceFree        104 //空闲

//框架
#define MainGameFrame           400 //游戏框架
#define SubGameFrameStart       101 //开始
#define SubGameFramePlaying     102 //游戏中(下注)
#define SubGameFrameOver        103 //开奖
#define SubGameFrameBetResult   104 //下注结果
#define SubGameFrameCheckout    105 //下注结果
#define SubGameFrameSetHost     106 //定庄
#define SubGameFrameHost        107 //抢庄
#define SubGameFrameSuperHost   108 //超级抢庄
#define SubGameFrameResult      109 //结果信息
#define SubGameFrameReady       110 //准备

//游戏状态
#define MainGameState           401 //游戏状态
#define SubGameStateStart       101 //开始
#define SubGameStatePlaying     102 //游戏中(下注)
#define SubGameStateOver        103 //开奖
#define SubGameStateCall        104 //叫分

//玩家状态
#define MainPlayerState         500 //玩家状态
#define PlayerSitDown           1   //坐下
#define PlayerAgree             2   //同意
#define PlayerAction            3   //游戏
#define PlayerStandUp           4   //站起
#define PlayerLookOn            5   //旁观

//更新标识
#define MainGameUpdate          501 //游戏状态
#define GameUpdatePlayerList    0   //玩家列表
#define GameUpdateHost          1   //玩家抢庄
#define GameUpdateSuperHost     2   //玩家超级抢庄
#define GameUpdateOut           3   //玩家出场
#define GameUpdateOffline       4   //玩家离线
#define GameUpdateReconnect     5   //玩家重连
#define GameUpdateReady         6   //玩家准备

//源码中重要标识
#define INVALID                 -1 //无效(切记有效初始化,不要从零开始)
#define FAILD                   0 //失败
#define SUCCESS                 1 //成功

//房间级别
#define RoomGeneral             0 //普通
#define RoomMiddle              1 //中级
#define RoomHigh                2 //高级
#endif
