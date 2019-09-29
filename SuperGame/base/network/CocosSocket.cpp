#include "CocosSocket.h"

#include <mutex>
#include <sstream>
#include <winsock.h>

CocosSocket *CocosSocket::m_pSingleton = nullptr;
CocosSocket::CocosSocket()
{
	// ��ʼ��
	Clear();
}

CocosSocket::~CocosSocket()
{
	Destroy();
}

CocosSocket * CocosSocket::GetInstance()
{
	if (m_pSingleton == nullptr) {
		std::mutex varMutex;
		std::lock_guard<std::mutex>lock(varMutex);  // �Խ���
		if (m_pSingleton == nullptr) {
			m_pSingleton = new CocosSocket();
		}
	}
	return m_pSingleton;
}




void CocosSocket::closeSocket()
{
#ifdef WIN32
	closesocket(m_sockClient);
	WSACleanup();
#else
	close(m_sockClient);
#endif
}

bool CocosSocket::Create(const char* pszServerIP, int nServerPort, int nBlockSec, bool bKeepAlive /*= FALSE*/)
{
	// ������
	if (pszServerIP == 0 || strlen(pszServerIP) > 15) {
		return false;
	}

#ifdef WIN32
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret != 0) {
		return false;
	}
#endif

	// �������׽���
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sockClient == INVALID_SOCKET) {
		closeSocket();
		return false;
	}

	// ����SOCKETΪKEEPALIVE
	if (bKeepAlive)
	{
		int		optval = 1;
		if (setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval)))
		{
			closeSocket();
			return false;
		}
	}

#ifdef WIN32
	DWORD nMode = 1;
	int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);
	if (nRes == SOCKET_ERROR) {
		closeSocket();
		return false;
	}
#else
	// ����Ϊ��������ʽ
	fcntl(m_sockClient, F_SETFL, O_NONBLOCK);
#endif

	unsigned long serveraddr = inet_addr(pszServerIP);
	if (serveraddr == INADDR_NONE)	// ���IP��ַ��ʽ����
	{
		closeSocket();
		return false;
	}

	sockaddr_in	addr_in;
	memset((void *)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(nServerPort);
	addr_in.sin_addr.s_addr = serveraddr;

	if (connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
		if (hasError()) {
			closeSocket();
			return false;
		}
		else	// WSAWOLDBLOCK
		{
			timeval timeout;
			timeout.tv_sec = nBlockSec;
			timeout.tv_usec = 0;
			fd_set writeset, exceptset;
			FD_ZERO(&writeset);
			FD_ZERO(&exceptset);
			FD_SET(m_sockClient, &writeset);
			FD_SET(m_sockClient, &exceptset);

			int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
			if (ret == 0 || ret < 0) {
				closeSocket();
				return false;
			}
			else	// ret > 0
			{
				ret = FD_ISSET(m_sockClient, &exceptset);
				if (ret)		// or (!FD_ISSET(m_sockClient, &writeset)
				{
					closeSocket();
					return false;
				}
			}
		}
	}

	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	return true;
}

bool CocosSocket::SendMsg(void* pBuf, int nSize)
{
	if (pBuf == 0 || nSize <= 0) {
		return false;
	}

	if (m_sockClient == INVALID_SOCKET|| !Check()) {
		return false;
	}

	// ���ͨѶ��Ϣ������
	int packsize = 0;
	packsize = nSize;

	// ���BUF���
	if (m_nOutbufLen + nSize > OUTBUFSIZE) {
		// ��������OUTBUF�е����ݣ������OUTBUF��
		sendToSock();
		if (m_nOutbufLen + nSize > OUTBUFSIZE) {
			// ������
			Destroy();
			return false;
		}
	}
	// ������ӵ�BUFβ
	memcpy(m_bufOutput + m_nOutbufLen, pBuf, nSize);
	m_nOutbufLen += nSize;
	sendToSock();
	return true;
}

bool CocosSocket::RecvMsg(void* pBuf, int& nSize)
{
	//������
	if (pBuf == NULL) {
		return false;
	}

	if (m_sockClient == INVALID_SOCKET || !Check()) {
		return false;
	}

	// �����Ҫ ����������ղ��� -----Start
	m_nInbufLen = 0;
	memset(m_bufInput, 0, sizeof(m_bufInput));
	// -------------------------------End


	// ����Ƿ���һ����Ϣ(С��2���޷���ȡ����Ϣ����)
	if (m_nInbufLen < 2) {
		//  ���û������ɹ�  ����   ���û��������ֱ�ӷ���
		if (!recvFromSock() || m_nInbufLen < 2) {        // ���m_nInbufLen������
			return false;
		}
	}


	//
	memcpy((unsigned char *)pBuf, m_bufInput, m_nInbufLen);
	nSize = m_nInbufLen;
	return true;


	//-----�������²���-------
	// ����Ҫ��������Ϣ�Ĵ�С��һ����Ϣ����СΪ������Ϣ�ĵ�һ��16�ֽڣ�����Ϊ���λ�����������Ҫ�ֿ�����
	int packsize = (unsigned char)m_bufInput[m_nInbufStart] +
		(unsigned char)m_bufInput[(m_nInbufStart + 1) % INBUFSIZE] * 256; // ע���ֽ��򣬸�λ+��λ

																		  // �����Ϣ���ߴ���� �ݶ����16k
	if (packsize <= 0 || packsize > _MAX_MSGSIZE) {
		m_nInbufLen = 0;        // ֱ�����INBUF
		m_nInbufStart = 0;
		return false;
	}

	// �����Ϣ�Ƿ�����(�����Ҫ��������Ϣ���ڴ�ʱ���������ݳ��ȣ���Ҫ�ٴ��������ʣ������)
	if (packsize > m_nInbufLen) {
		// ���û������ɹ�   ����    ��Ȼ�޷���ȡ�����������ݰ�  �򷵻أ�ֱ��ȡ��������
		if (!recvFromSock() || packsize > m_nInbufLen) {    // ���m_nInbufLen�Ѹ���
			return false;
		}
	}

	// ���Ƴ�һ����Ϣ
	if (m_nInbufStart + packsize > INBUFSIZE) {
		// ���һ����Ϣ�лؾ�����������ڻ��λ�������ͷβ��
		// �ȿ������λ�����ĩβ������
		int copylen = INBUFSIZE - m_nInbufStart;
		memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);

		// �ٿ������λ�����ͷ����ʣ�ಿ��
		memcpy((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);
		nSize = packsize;
	}
	else {
		// ��Ϣû�лؾ�����һ�ο�����ȥ
		memcpy(pBuf, m_bufInput + m_nInbufStart, packsize);
		nSize = packsize;
	}

	// ���¼��㻷�λ�����ͷ��λ��
	m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
	m_nInbufLen -= packsize;
}

bool CocosSocket::hasError()
{
	return WSAGetLastError() != WSAEWOULDBLOCK;
//#ifdef WIN32
//	int err = WSAGetLastError();
//	if (err != WSAEWOULDBLOCK) {
//#else
//	int err = errno;
//	if (err != EINPROGRESS && err != EAGAIN) {
//#endif
//		return true;
//	}
//	return false;
}

// �������з��;����ܶ������ 
bool CocosSocket::sendToSock(void)//? ��� OUTBUF > SENDBUF ����Ҫ���SEND����
{
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	if (m_nOutbufLen <= 0) {
		m_nOutbufLen = 0;
		memset(m_bufOutput, 0, sizeof(m_bufOutput));
		return true;
	}

	// ����һ������
	int	outsize;
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
	if (outsize > 0) {
		// ɾ���ѷ��͵Ĳ���
		if (m_nOutbufLen - outsize > 0) {
			memcpy(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);
		}

		m_nOutbufLen -= outsize;

		if (m_nOutbufLen < 0) {
			return false;
		}
	}
	else {
		if (hasError()) {
			Destroy();
			return false;
		}
	}

	return true;
}


// �������ж�ȡ�����ܶ�����ݣ�ʵ����������������ݵĵط�
bool CocosSocket::recvFromSock(void)
{
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) {
		return false;
	}

	// ���յ�һ������
	int	savelen, savepos;			// ����Ҫ����ĳ��Ⱥ�λ��
	if (m_nInbufStart + m_nInbufLen < INBUFSIZE) {	// INBUF�е�ʣ��ռ��л���
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);		// �󲿿ռ䳤�ȣ����������ݵĳ���
	}
	else {
		savelen = INBUFSIZE - m_nInbufLen;
	}

	// ���������ݵ�ĩβ
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
	//	CHECKF(savepos + savelen <= INBUFSIZE);
	int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
	if (inlen > 0) {
		// �н��յ�����
		m_nInbufLen += inlen;

		if (m_nInbufLen > INBUFSIZE) {
			return false;
		}

		// ���յڶ�������(һ�ν���û����ɣ����յڶ�������)
		if (inlen == savelen && m_nInbufLen < INBUFSIZE) {
			int savelen = INBUFSIZE - m_nInbufLen;
			int savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
			//CHECKF(savepos + savelen <= INBUFSIZE);
			inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
			if (inlen > 0) {
				m_nInbufLen += inlen;
				if (m_nInbufLen > INBUFSIZE) {
					return false;
				}
			}
			else if (inlen == 0) {
				Destroy();
				return false;
			}
			else {
				// �����ѶϿ����ߴ��󣨰���������
				if (hasError()) {
					Destroy();
					return false;
				}
			}
		}
	}
	else if (inlen == 0) {
		Destroy();
		return false;
	}
	else {
		// �����ѶϿ����ߴ��󣨰���������
		if (hasError()) {
			Destroy();
			return false;
		}
	}
	return true;
}


bool CocosSocket::Check(void)
{
	// ���״̬
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	char buf[1];
	int	ret = recv(m_sockClient, buf, 1, MSG_PEEK);
	if (ret == 0) {
		Destroy();
		return false;
	}
	else if (ret < 0) {
		if (hasError()) {
			Destroy();
			return false;
		}
		else {	// ����
			return true;
		}
	}
	else {	// ������
		return true;
	}

	return true;
}

void CocosSocket::Destroy(void)
{
	// �ر�
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	int ret = setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	closeSocket();

	m_sockClient = INVALID_SOCKET;
	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	Clear();
}

void CocosSocket::Clear()
{
	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
}

CocosSocket::GC::~GC()
{
	//�����ھ����ɾ��
	if (m_pSingleton != NULL) {
		delete m_pSingleton;
		m_pSingleton = NULL;
	}
}
