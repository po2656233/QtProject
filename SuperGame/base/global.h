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
//#define ROOM_TYPE_Fight	1
//#define ROOM_TYPE_Multiplayer 2
//#define ROOM_TYPE_SameTable   3
//#define ROOM_TYPE_Guess	4
//#define ROOM_TYPE_GamesCity  5
//#define ROOM_TYPE_DualMeet   6
//#define ROOM_TYPE_Sport 	7
//#define ROOM_TYPE_Smart	8
#define ROOM_NUM_Multiplayer	101	//

#define ROOM_NUM_Fight	102	// ?????	6	20000
#define ROOM_NUM_DualMeet	103	// ?羺??	7	20000
#define ROOM_NUM_SameTable	104	// ??????	4,5,6	20000
#define ROOM_NUM_GamesCity	105	// ??????	3	20000
#define ROOM_NUM_Guess	106	// ????	4	20000
#define ROOM_NUM_Smart	107	// ????	4	20000

///////////////////->GAME<-////////////////////////////////
//KINDID
#define GAME_COWCOW		1001
#define GAME_HKFIVE		1002
#define GAME_ZHAJINHUA	2003
#define GAME_BACCARAT	2001
#define GAME_TOUBAO		2002
#define GAME_LANDLORD	3001
#define GAME_MAHJONG	3002
#define GAME_ChinessChess 8002

//MainID 200+ || SubID 100+
//???
#define MainRegister 200					//-> ???
#define SubRegister 101						//-> ???
//???
#define MainLogin 201						//-> ???
#define SubLoginResult 101 					//-> ???????
#define SubMasterInfo 102 					//-> ??????
#define SubGameList 103 					//-> ????б?
#define SubEnterRoomResult 104				//??????????
#define SubEnterGameResult 105				//???????????
//????
#define MainGameSence 300					//???????
#define SubGameSenceStart 101				//???
#define SubGameSencePlaying 102				//?????
#define SubGameSenceOver 103				//???????
#define SubGameSenceFree 104				//????
//????
#define MainGameFrame 400					//???????
#define SubGameFrameStart 101				//???
#define SubGameFramePlaying 102				//?????(???)
#define SubGameFrameOver 103				//????
#define SubGameFrameBetResult 104			//???????
#define SubGameFrameCheckout 105			//???????
#define SubGameFrameSetHost 106				//???
#define SubGameFrameHost 107					//???
#define SubGameFrameSuperHost 108			//???????
#define SubGameFrameResult 109					//???????
#define SubGameFrameReady 110               //准备
//?????
#define MainGameState 401					//?????
#define SubGameStateStart 101				//???
#define SubGameStatePlaying 102				//?????(???)
#define SubGameStateOver 103				//????
#define subGameStateCall 104                //
//??????
#define MainPlayerState 500					//??????
#define PlayerLookOn 0						//???
#define PlayerSitDown 1						//????
#define PlayerAgree 2						//???
#define PlayerAction 3						//???
#define PlayerStandUp 4						//???
//???±??
#define MainGameUpdate 501					//?????
#define GameUpdatePlayerList 0				//?????б?
//???????????
#define FAILD 0								//???
#define SUCCESS 1							//???



#endif
