#ifndef _XNET_TIMER_H
#define _XNET_TIMER_H

#include <process.h>
#include <direct.h>
#include <windows.h>

#include <list>
#include "lang/SafeQueue.h"
#include "xnet/ThreadList.h"

/*!
	定时器对象,用于定时向主线程发送类型为mtTimer的消息
	-	该对象是系统分配的线程安全的唯一一个全局定时器对象
	-	在开始使用定时器之前,必须设置定时间隔和主线程所用消息队列
*/

#define WM_TIMER_MSG WM_USER+40

namespace lang {

typedef xnet::TemplateMessageList<lang::TMessage *> TTimeMessage;

class TTimer
{
	typedef struct {
		bool Onece;
		bool WaitAfterGet;
		int Count;
		int p1;
		int p2;
		int Tick;
	} TTimerItem;

private:
	TTimeMessage *FMainQueue;

    CRITICAL_SECTION FMutex;
	DWORD FThreadID;
    DWORD pt;
 	HANDLE handle;

    static DWORD WINAPI TimerCreateThread(void *owner);

	std::list <TTimerItem> FList;
	int FInterval;

public:
	enum { mtTimer = 20 };

	/*!
		构造函数
		参数:
		-	MainQueue, 用于存放定时事件的消息队列
		-	Interval, 统一的最小时间间隔
		-	new TTimer之后需要调用Active()使定时器生效
	*/
	TTimer(TTimeMessage *MainQueue, int Interval);

	TTimer(DWORD threadid, int Interval);

	/*!
		必须调用析构函数,保证线程能正常结束
	*/
	virtual ~TTimer();

	/*!
		线程执行体函数,不能直接调用
	*/
	void Execute();

	/*!
		定时器生效
	*/
	void Active();

	/*!
		设置一个定时器
		-	输入: Count 间隔秒数, p1, p2 自定义标识, Onece 是否仅发一次消息
		-	p1,p2在一个进程中必须统一分配，不能重复
		-	典型用法: p1为一个业务类型, p2为业务机构号
		-	定时器到触发条件时，向MainQueue发送一个TMessage Message，其中
		-		Message.Type = mtTimer;
		-		Message.Param[0] = Count;
		-		Message.Param[1] = p1;
		-		Message.Param[2] = p2;
		-		Message.Param[3] = (int) this;
		-	主程序根据Param[1],Param[2]去调用相应的处理程序
	*/
	void SetTimer(int Count, int p1, int p2, bool Onece);

	/*!
		停止某个定时器设置
		-	输入: p1,p2, 上次设置定时器使用的自定义标识
	*/
	void StopTimer(int p1, int p2);

	/*!
		清除所有定时设置
	*/
	void Clear();

	/*!
		当主线程读到TTimer的消息时，调用AfterGet来重新激活该时钟，防止超时重入,
		非程序员调用!
	*/
	void AfterGet(int p1, int p2);
};
	
}
#endif
