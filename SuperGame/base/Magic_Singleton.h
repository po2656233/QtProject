////////////////////////////////////////////////////
/// 版权(@copyright):  智慧中华(smart china)技术有限公司
/// 文件(@interface):  Magic_Singleton.h
/// 版本(@version):    1.0
/// 作者(@author):     Song
/// 日期(@date)  :
/// 功能(@brief) ：    万能单例类
/// 目的(@target)：    可把任何类包装成线程安全的全局单例类，出口默认智能指针
/// 备注(@attention): songshuda@vip.qq.com
////////////////////////////////////////////////////


#ifndef MAGIC_SINGLETON_H
#define MAGIC_SINGLETON_H

#include <mutex>
#include <memory>

template<typename T>
class Magic_Singleton {
public:

	//获取全局单例对象
	template<typename ...Args>
	static std::shared_ptr<T> GetInstance(Args&&... args) {
		if (!m_pSington) {
			std::lock_guard<std::mutex> gLock(m_Mutex);
			if (nullptr == m_pSington) {
				m_pSington = std::make_shared<T>(std::forward<Args>(args)...);
			}
		}
		return m_pSington;
	}

	//主动析构单例对象（一般不需要主动析构，除非特殊需求）
	static void DesInstance() {
		if (m_pSington) {
			m_pSington.reset();
			m_pSington = nullptr;
		}
	}

private:
	explicit Magic_Singleton();
	Magic_Singleton(const Magic_Singleton&) = delete;
	Magic_Singleton& operator=(const Magic_Singleton&) = delete;
	~Magic_Singleton() {}

private:
	static std::shared_ptr<T> m_pSington;
	static std::mutex m_Mutex;
};

template<typename T>
std::shared_ptr<T> Magic_Singleton<T>::m_pSington = nullptr;

template<typename T>
std::mutex Magic_Singleton<T>::m_Mutex;


//调用实例-->
////test.cpp
//#include <iostream>
//#include "Magic_Singleton.h"
//
//using namespace std;
//
//class MyClass {
//public:
//	MyClass(const string& strData) : m_strData(strData) {
//		cout << m_strData.data() << endl;
//	};
//	~MyClass() {
//		cout << "destory" << endl;
//	};
//
//private:
//	string m_strData;
//};
//
//auto main()->int { //等同于入口函数int main()
//	auto pClass = Magic_Singleton<MyClass>::GetInstance("create");
//
//	Magic_Singleton<MyClass>::DesInstance();
//
//	return 1;
//}


#endif
