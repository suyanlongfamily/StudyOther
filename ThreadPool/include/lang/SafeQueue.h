#ifndef _XNET_SAFEQUEUE_H
#define _XNET_SAFEQUEUE_H
/*
 * �̰߳�ȫ��ģ���
 * 		����C++ Standard Library ��Posix���̱߳��
 *			����:��־��, 2004.4.4
 *          �޸ģ��Ը��ʣ�2007.12.18
 *          �޸ģ���������2008.12.3
 *			�޸ģ���������2016.6.23
 */

#include <list>
#include <time.h>
#include <windows.h>

/*!
	���߳�ͨѶ����Ϣ����ģ��,��Ϣ����T����Ϊ������������

	Ӧ�÷�Χ:
	-	���Խ����ݼ򵥵Ĵ��������߳��ƽ���ʹ�����߳�
	-	��NT���̱߳��ʱ,������д�̺߳Ͷ�����߳�ͬʱ����,
		���ҿ�����û����Ҫ���������ʱ,�ö��̴߳�������״̬,�Լ���CPUռ��.

	ԭ��:
	-	���ǽ���һ����Ϣ����,�����ݱ���������ݵĵ�ַ��Ϊ��ϢT,ʹ���̻߳�����ź����������̰߳�ȫ�Ĳ���.
	-	�����ݲ�����һ��long int���͵ĳ���ʱ,����Ҫ������ڴ����
	-	�����ݳ���һ��long int���͵ĳ���ʱ,	�������������ڴ�,ʹ��PostMessage�������ݵĵ�ַ��Ϊ��Ϣ���
		������,Ȼ�������߾�ʧȥ���ڴ�Ŀ���Ȩ. ���߳�(ʹ����)ʹ��GetMessage����ȡ��Ϣ����ȡ���ڴ�Ŀ���
		Ȩ,������Ϻ����ͷ��ڴ�.

	ע��:
	-	ʹ�ñ�ģ�������ѭ������/ʹ���ߵ��ڴ����ԭ��.

	��T�Ľ���:
	-	����������(<=sizeof(long)),Tֱ��������.
	-	����ǳ��������ͻ��߶���,Tһ��Ҫ��ָ��,��Ϊ���������Լ���������,��PostMessage֮ǰ
		һ��Ҫnew,GetMessage֮��һ��Ҫdelete.

	ʹ�þ���:
	-	ʹ��TemplateMessageQueue <pthread_t> MyMessageQueue;��������һ������߳̾������Ϣ����.
	-	ʹ��TemplateMessageQueue <char *> MyCharMsgQ;��������һ�����buffer����Ϣ����.
*/

namespace lang {

template<class T> class TemplateMessageQueue {
private:
	std::list<T> FList;
	CRITICAL_SECTION FMutex;
	time_t FLastTime;

	HANDLE handle;
public:

	TemplateMessageQueue ()
	{
		InitializeCriticalSection( &FMutex);
		FList.clear();

		handle = CreateEvent(
			NULL,                   // default security attributes
			FALSE,                  // manual-reset event
			FALSE,                  // initial state is signaled
			TEXT("WriteEvent")      // object name
		);
	};

	void InitEvent(std::string eventname)
	{
		handle = CreateEvent(
			NULL,                   // default security attributes
			FALSE,                  // manual-reset event
			FALSE,                  // initial state is signaled
			TEXT(eventname.c_str()) // object name
		);
	}

	~TemplateMessageQueue ()
	{
		DeleteCriticalSection(&FMutex);
	};
	/*!
		Lock ����
	*/
	void Lock()
	{
		EnterCriticalSection( &FMutex );
	}
	/*!
		UnLock ����
	*/
	void UnLock()
	{
		LeaveCriticalSection( &FMutex );
	}
	/*!
		Wait ����
	*/
	void Wait()
	{
		WaitForSingleObject(handle,INFINITE);
	}
	/*!
		Resume ����
	*/
	void Resume()
	{
		SetEvent(handle);
	}
	/*!
		����Ϣ�����л�ȡ����,Ϊ��ʱ�߳���������ʽ�ȴ�,���ٶ�CPU��ռ��
	*/
	T GetMessage ()
	{
		//bool isempty = FList.empty();
		T result = NULL;
		if (FList.empty())
		{
			Wait();
		}
		//while(1)
		//{
		//	if (!FList.empty())
		//		break;
		//	Wait();
		//}	

		Lock();
		if (!FList.empty())	//��ֹ������ʧЧ
		{	
			result = FList.front ();
			FList.pop_front ();
		} 
		UnLock();
		time (&FLastTime);
		return result;
	};
	/*!
		�������ݵ���Ϣ����,�ڶ���ԭ��Ϊ��ʱ,�Զ����������Ķ��߳�
	*/
	void PostMessage (T message)
	{
		Lock();
		FList.push_back (message);
		UnLock();
		Resume();
	};
	/*!
		����ģ������ԭlist,�����������
	*/
	std::list<T>  List () { return FList; }
	/*!
		���ض��߳����һ�ζ���Ϣ��ʱ��
	*/
	const time_t LastTime () { return (const time_t) FLastTime; }
};

typedef struct {
	/*!
		��Ϣ����
		-	������Ӧ�ó�����,��Ϣ���ͱ���ͳһ����
	*/
	int Type;
	/*!
		��Ϣ����
	*/
	long Param[4];
} TMessage;

typedef TemplateMessageQueue <TMessage *> TMessageQueue;

} //namespace lang

#endif //#define _LANG_SAFEQUEUE_H
