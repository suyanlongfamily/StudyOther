#ifndef _XNET_TIMER_H
#define _XNET_TIMER_H

#include <process.h>
#include <direct.h>
#include <windows.h>

#include <list>
#include "lang/SafeQueue.h"
#include "xnet/ThreadList.h"

/*!
	��ʱ������,���ڶ�ʱ�����̷߳�������ΪmtTimer����Ϣ
	-	�ö�����ϵͳ������̰߳�ȫ��Ψһһ��ȫ�ֶ�ʱ������
	-	�ڿ�ʼʹ�ö�ʱ��֮ǰ,�������ö�ʱ��������߳�������Ϣ����
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
		���캯��
		����:
		-	MainQueue, ���ڴ�Ŷ�ʱ�¼�����Ϣ����
		-	Interval, ͳһ����Сʱ����
		-	new TTimer֮����Ҫ����Active()ʹ��ʱ����Ч
	*/
	TTimer(TTimeMessage *MainQueue, int Interval);

	TTimer(DWORD threadid, int Interval);

	/*!
		���������������,��֤�߳�����������
	*/
	virtual ~TTimer();

	/*!
		�߳�ִ���庯��,����ֱ�ӵ���
	*/
	void Execute();

	/*!
		��ʱ����Ч
	*/
	void Active();

	/*!
		����һ����ʱ��
		-	����: Count �������, p1, p2 �Զ����ʶ, Onece �Ƿ����һ����Ϣ
		-	p1,p2��һ�������б���ͳһ���䣬�����ظ�
		-	�����÷�: p1Ϊһ��ҵ������, p2Ϊҵ�������
		-	��ʱ������������ʱ����MainQueue����һ��TMessage Message������
		-		Message.Type = mtTimer;
		-		Message.Param[0] = Count;
		-		Message.Param[1] = p1;
		-		Message.Param[2] = p2;
		-		Message.Param[3] = (int) this;
		-	���������Param[1],Param[2]ȥ������Ӧ�Ĵ������
	*/
	void SetTimer(int Count, int p1, int p2, bool Onece);

	/*!
		ֹͣĳ����ʱ������
		-	����: p1,p2, �ϴ����ö�ʱ��ʹ�õ��Զ����ʶ
	*/
	void StopTimer(int p1, int p2);

	/*!
		������ж�ʱ����
	*/
	void Clear();

	/*!
		�����̶߳���TTimer����Ϣʱ������AfterGet�����¼����ʱ�ӣ���ֹ��ʱ����,
		�ǳ���Ա����!
	*/
	void AfterGet(int p1, int p2);
};
	
}
#endif
