////////////////////////////////////////////////////
/// ��Ȩ(@copyright):  �ǻ��л�(smart china)�������޹�˾
/// �ļ�(@interface):  Magic_Singleton.h
/// �汾(@version):    1.0
/// ����(@author):     Song
/// ����(@date)  :
/// ����(@brief) ��    ���ܵ�����
/// Ŀ��(@target)��    �ɰ��κ����װ���̰߳�ȫ��ȫ�ֵ����࣬����Ĭ������ָ��
/// ��ע(@attention): songshuda@vip.qq.com
////////////////////////////////////////////////////


#ifndef MAGIC_SINGLETON_H
#define MAGIC_SINGLETON_H

#include <mutex>
#include <memory>

template<typename T>
class Magic_Singleton {
public:

	//��ȡȫ�ֵ�������
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

	//����������������һ�㲻��Ҫ����������������������
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


//����ʵ��-->
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
//auto main()->int { //��ͬ����ں���int main()
//	auto pClass = Magic_Singleton<MyClass>::GetInstance("create");
//
//	Magic_Singleton<MyClass>::DesInstance();
//
//	return 1;
//}


#endif
