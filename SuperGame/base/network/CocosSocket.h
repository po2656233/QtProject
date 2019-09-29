#ifndef __COCOSSOCKET_H__
#define __COCOSSOCKET_H__
#include "base/global.h"

#include <windows.h>
#include <WinSock.h>


#ifndef CHECKF
#define CHECKF(x) \
	do \
{ \
	if (!(x)) { \
	log_msg("CHECKF", #x, __FILE__, __LINE__); \
	return 0; \
	} \
} while (0)
#endif



class Message
{
public:
	struct MsgHead     //头消息
	{
		int msgId;    //消息标识
        MsgHead(int msg = INVALID_MSG) :msgId(msg) {}
	};
	struct MsgFileLength :public MsgHead
	{
        long long fileLength;   //文件长度
		MsgFileLength() :MsgHead(MSG_FILE_LENGTH) {}
	};
	struct MsgFileName :public MsgHead
	{
		char fileName[MAX_FILE_NAME_LENGTH];
		MsgFileName() :MsgHead(MSG_FILE_NAME) {}
	};
	struct MsgFile :public MsgHead
	{
		MsgFile() :MsgHead(MSG_FILE) {}
	};
	struct MsgReady :public MsgHead        //准备好消息
	{
		MsgReady() :MsgHead(MSG_READY) {}
	};
	struct MsgSendFile :public MsgHead  //发送文件消息
	{
		MsgSendFile() :MsgHead(MSG_SEND_FILE) {}
	};
	struct MsgDownLoadFile :public MsgHead     //下载文件消息
	{
		MsgDownLoadFile() :MsgHead(MSG_DOWNLOAD_FILE) {}
	};
	struct MsgComplete :public MsgHead
	{
		MsgComplete() :MsgHead(MSG_COMPLETE) {}
	};
};


//网络通信类 socket
class CocosSocket {
public:
	static CocosSocket *GetInstance();//获取实例

	bool	Create(const char* pszServerIP, int nServerPort, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);	//建立链接
	bool	SendMsg(void* pBuf, int nSize);		//发送
	bool	RecvMsg(void* pBuf, int& nSize);	//接收

	
	bool	Check(void);						//网络状态检测
	void	Destroy(void);						//销毁资源
	void	Clear();							//清空缓存

	//对外提供socket
	inline SOCKET	GetSocket(void) const { return m_sockClient; }
private:
	CocosSocket(void);				// 构造函数（被保护）
	~CocosSocket(void);

	bool	sendToSock(void);		// 从网络中发送尽可能多的数据
	bool	recvFromSock(void);		// 从网络中读取尽可能多的数据
	bool    hasError();				// 是否发生错误，注意，异步模式未完成非错误
	void    closeSocket();

	SOCKET	m_sockClient;

	// 发送数据缓冲
	char	m_bufOutput[OUTBUFSIZE];	//? 可优化为指针数组
	int		m_nOutbufLen;

	// 环形缓冲区
	char	m_bufInput[INBUFSIZE];
	int		m_nInbufLen;
	int		m_nInbufStart;				// INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1)
	
	//单例
	static CocosSocket *m_pSingleton;  // 指向单例对象的指针

	// GC 机制(内部回收资源)
	class GC
	{
	public:
		~GC();
		static GC gc;  // 用于释放单例
	};
};
#endif //__COCOSSOCKET_H__
