////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface): common.h
/// 版本(@version):   1.0
/// 作者(@author):    pitter
/// 日期(@date)  :    2019-08-04
/// 功能(@brief) ：
/// 目的(@target)：
/// 备注(@attention):
////////////////////////////////////////////////////
#ifndef COMMON_H
#define COMMON_H


/////////////////Game Base/////////////////////////////////
const unsigned char CardData[]={
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //方块 A - K
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //梅花 A - K
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //红桃 A - K
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //黑桃 A - K ------
};


/////////////////Network Base/////////////////////////////////
//bool isBigEndian();
//char * marshal(int len, int id, void * data, size_t & size);
//char * unmarshal(const void * data, size_t len, size_t & size);
#define _SMALL_SIZE 64				//
#define _MIN_SIZE 128				//
#define _GENERAL_SIZE 256			//
#define _BIG_SIZE 1024				//
#define _MAX_MSGSIZE 16 * 1024		// ??????????????16k
#define BLOCKSECONDS	30			// INIT???????????
#define INBUFSIZE	(64*1024)		//?	??????????????汨??????  ????????????
#define OUTBUFSIZE	(8*1024)		//? ??????????????汨???????? ???????????棬????????8K???FLUSH????SEND???

#define MAX_PACK_SIZE  10240        //??????????
#define MAX_FILE_NAME_LENGTH 256    //??????????
#define INVALID_MSG -1              //??Ч?????
#define MSG_FILE_LENGTH 1           //???????
#define MSG_FILE_NAME 2             //?????
#define MSG_FILE   4                //???????
#define MSG_READY   3               //????????
#define MSG_SEND_FILE 5             //???????
#define MSG_DOWNLOAD_FILE 6         //???????
#define MSG_COMPLETE 7              //???????
//////////////////////////////////////////////////////////////
namespace NSType
{
enum class enChip
{
    OneRMB = 0,
    TwoRMB,
    FiveRMB,
    TwentyRMB,
    FiftyRMB,
    HundredRMB,
    ZeroRMB
};
}
//struct enDrag
//{
//    enum class Type{
//        Text = 1,
//        Picture,
//        Form,
//        Movice,
//        NoType
//    };
//    enDrag() {
//        memset(m_type.sz, 0, sizeof(m_type.sz));
//    }
//    enDrag (const enDrag &obj) {
//        this->m_type = obj.m_type;
//    }
//    void setType(Type type)
//    {
//        memset(m_type.sz, 0, sizeof(m_type.sz));
//        m_type.code = type;
//        switch (type) {
//        case Type::Text:
//            strcpy_s(m_type.sz,"application/x-text");
//            break;
//        case Type::Picture:
//            strcpy_s(m_type.sz,"application/x-picture");
//            break;
//        case Type::Movice:
//            strcpy_s(m_type.sz,"application/x-movice");
//            break;
//        case Type::Form:
//            strcpy_s(m_type.sz,"application/x-form");
//            break;
//        default:
//            m_type.code = Type::NoType;
//            break;
//        }
//    }
//    inline Type getType()const{return m_type.code;}
//    inline const char* getTypeName()const{return m_type.sz;}

//private:
//    union unType{
//        Type code;
//        char sz[64];
//    };
//    unType m_type;
//};
//}

namespace NSNetwork
{
#include <string.h>
inline bool isBigEndian() {
    union
    {
        short i;
        char a[2];
    }u;
    u.a[0] = 0x11;
    u.a[1] = 0x22;
    return (u.i == 0x1122);
}


// 序列化 -->配合服务端规则  len+id+data   返回字节大小 和 数据
inline char * marshal(int len, int id, void * data, size_t & size)
{
    if (nullptr == data || 0 == len || _MAX_MSGSIZE < len) return nullptr;

    //???????η???????
    //???????????????????????? ????м???\0???c?????????
    int short_size = sizeof(unsigned short);

    static char szData[_MAX_MSGSIZE] = { 0 };
    memset(szData, 0, sizeof(szData));
    int totalLen = len + short_size;


    memcpy(szData, &totalLen, short_size);
    memcpy(szData + short_size, &id, short_size);
    memcpy(szData + 2 * short_size, data, len);

    //??С????? //0x2211 ?С??  0x1122 ?????
    if (!isBigEndian())
    {
        //С?????????
        char temp;
        temp = szData[0];
        szData[0] = szData[1];
        szData[1] = temp;

        temp = szData[2];
        szData[2] = szData[3];
        szData[3] = temp;
    }
    size = totalLen + short_size;
    return szData;
}

// 反序列 	len+id+data   返回字节大小 和 数据
inline char * unmarshal(const void * data, size_t len, size_t & size)
{
    if (nullptr == data || 0 == len || _MAX_MSGSIZE < len) return nullptr;

    static char szData[_MAX_MSGSIZE] = { 0 };
    memset(szData, 0, sizeof(szData));
    memcpy(szData, data, len);


    // ?????????len
    size_t nLen = 0;
    //int nID = 0;
    if (isBigEndian())
    {//????
        nLen =  (szData[1] & 0xff) << 8| (szData[0] & 0xff) << 0;
        // nID = (szData[3] & 0xff) << 8| (szData[2] & 0xff) << 0;;
    }
    else
    {//С??
        nLen =  (szData[0] & 0xff) << 8| (szData[1] & 0xff) << 0;
        //nID = (szData[2] & 0xff) << 8| (szData[3] & 0xff) << 0;;
    }
    if (_MAX_MSGSIZE < nLen || nLen <= 0)return nullptr;
    size = nLen - sizeof(unsigned short);
    if(_MAX_MSGSIZE < size || size <= 0)
    {
        return nullptr;
    }

    // skip the size of id.
    memcpy(szData, szData + 2 * sizeof(unsigned short), size);
    return szData;
}

}



///
//namespace NSBaseDevice {
//#include <QStringList>
//#include <QString>
//#include <qdebug.h>
//#include <QNetworkInterface>
//#ifdef __GNUC__
//#include <cpuid.h>
//#elif defined(_MSC_VER)
//#if _MSC_VER >= 1400
//#include <intrin.h>
//#endif
//#else
//#error Only supports MSVC or GCC
//#endif
//inline QStringList get_mac()
//{
//    QStringList mac_list;
//    QString strMac;
//    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
//    for (int i=0; i<ifaces.count(); i++)
//    {
//        QNetworkInterface iface = ifaces.at(i);
//        //??????????????????п???????
//        if (iface.flags().testFlag(QNetworkInterface::IsUp) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
//        {
//            //????????????
//            if (!(iface.humanReadableName().contains("VMware",Qt::CaseInsensitive)))
//            {
//                strMac = iface.hardwareAddress();
//                mac_list.append(strMac);
//            }
//        }
//    }
//    return mac_list;
//}



//inline void getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
//{
//#if defined(__GNUC__)// GCC
//    __cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
//#elif defined(_MSC_VER)// MSVC
//#if _MSC_VER >= 1400 //VC2005?????__cpuid
//    __cpuid((int*)(void*)CPUInfo, (int)(InfoType));
//#else //???????getcpuidex
//    getcpuidex(CPUInfo, InfoType, 0);
//#endif
//#endif
//}


//inline QString get_cpuId()
//{
//    QString cpu_id = "";
//    unsigned int dwBuf[4];
//    unsigned long long ret;

//    auto getcpuid = [=](unsigned int CPUInfo[4], unsigned int InfoType){
//#if defined(__GNUC__)// GCC
//        __cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
//#elif defined(_MSC_VER)// MSVC
//#if _MSC_VER >= 1400 //VC2005?????__cpuid
//        __cpuid((int*)(void*)CPUInfo, (int)(InfoType));
//#else //???????getcpuidex
//        getcpuidex(CPUInfo, InfoType, 0);
//#endif
//#endif
//    };


//    getcpuid(dwBuf, 1);
//    ret = dwBuf[3];
//    ret = ret << 32;
//    cpu_id = QString::number(dwBuf[3], 16).toUpper();
//    cpu_id = cpu_id + QString::number(dwBuf[0], 16).toUpper();
//    return cpu_id;
//}


//inline void getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
//{
//#if defined(_MSC_VER) // MSVC
//#if defined(_WIN64)	// 64λ?2????????????. 1600: VS2010, ???VC2008 SP1????????__cpuidex.
//    __cpuidex((int*)(void*)CPUInfo, (int)InfoType, (int)ECXValue);
//#else
//    if (NULL==CPUInfo)	return;
//    _asm{
//        // load. ??????????????.
//        mov edi, CPUInfo;
//        mov eax, InfoType;
//        mov ecx, ECXValue;
//        // CPUID
//        cpuid;
//        // save. ??????????浽CPUInfo
//        mov	[edi], eax;
//        mov	[edi+4], ebx;
//        mov	[edi+8], ecx;
//        mov	[edi+12], edx;
//    }
//#endif
//#endif
//}

//}

#endif // COMMON_H
