#ifndef _XNET_SAFEQUEUE_H
#define _XNET_SAFEQUEUE_H
/*
 * 线程安全的模板库
 * 		基于C++ Standard Library 和Posix多线程编程
 *			作者:刘志军, 2004.4.4
 *          修改：赵高攀，2007.12.18
 *          修改：胡书敏，2008.12.3
 *			修改：苏彦龙，2016.6.23
 */

#include <list>
#include <time.h>
#include <windows.h>

/*!
	多线程通讯用消息队列模板,消息类型T可以为任意数据类型

	应用范围:
	-	可以将数据简单的从生产者线程移交给使用者线程
	-	在NT多线程编程时,允许多个写线程和多个读线程同时操作,
		并且可以在没有需要处理的数据时,让读线程处于阻塞状态,以减少CPU占用.

	原理:
	-	我们建立一个消息队列,把数据本身或者数据的地址作为消息T,使用线程互斥和信号量来进行线程安全的操作.
	-	当数据不超过一个long int类型的长度时,不需要额外的内存管理
	-	当数据超过一个long int类型的长度时,	由生产者申请内存,使用PostMessage将该数据的地址作为消息添加
		到队列,然后生产者就失去对内存的控制权. 读线程(使用者)使用GetMessage来读取消息并获取对内存的控制
		权,处理完毕后负责释放内存.

	注意:
	-	使用本模板必须遵循生产者/使用者的内存管理原则.

	对T的建议:
	-	短数据类型(<=sizeof(long)),T直接用类型.
	-	如果是长数据类型或者对象,T一定要用指针,因为变量都有自己的作用域,在PostMessage之前
		一定要new,GetMessage之后一定要delete.

	使用举例:
	-	使用TemplateMessageQueue <pthread_t> MyMessageQueue;可以申明一个存放线程句柄的消息队列.
	-	使用TemplateMessageQueue <char *> MyCharMsgQ;可以申明一个存放buffer的消息队列.
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
		Lock 队列
	*/
	void Lock()
	{
		EnterCriticalSection( &FMutex );
	}
	/*!
		UnLock 队列
	*/
	void UnLock()
	{
		LeaveCriticalSection( &FMutex );
	}
	/*!
		Wait 队列
	*/
	void Wait()
	{
		WaitForSingleObject(handle,INFINITE);
	}
	/*!
		Resume 队列
	*/
	void Resume()
	{
		SetEvent(handle);
	}
	/*!
		从消息队列中获取数据,为空时线程以阻塞方式等待,减少对CPU的占用
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
		if (!FList.empty())	//防止迭代器失效
		{	
			result = FList.front ();
			FList.pop_front ();
		} 
		UnLock();
		time (&FLastTime);
		return result;
	};
	/*!
		发送数据到消息队列,在队列原来为空时,自动唤醒阻塞的读线程
	*/
	void PostMessage (T message)
	{
		Lock();
		FList.push_back (message);
		UnLock();
		Resume();
	};
	/*!
		返回模板所用原list,便于特殊操作
	*/
	std::list<T>  List () { return FList; }
	/*!
		返回读线程最后一次读消息的时间
	*/
	const time_t LastTime () { return (const time_t) FLastTime; }
};

typedef struct {
	/*!
		消息类型
		-	在整个应用程序中,消息类型必须统一分配
	*/
	int Type;
	/*!
		消息参数
	*/
	long Param[4];
} TMessage;

typedef TemplateMessageQueue <TMessage *> TMessageQueue;

} //namespace lang

#endif //#define _LANG_SAFEQUEUE_H
