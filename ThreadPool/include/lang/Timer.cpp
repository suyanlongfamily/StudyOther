// TTimer.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include "lang/Timer.h"

namespace lang {

DWORD WINAPI TTimer::TimerCreateThread(void *owner)
{
	TTimer *Timer = (TTimer *)owner;
	Timer->Execute();
	return NULL;
}


TTimer::TTimer(TTimeMessage *MainQueue, int interval)
{
	FThreadID = 0;
	FMainQueue = MainQueue;

    InitializeCriticalSection( &FMutex);

	if (interval < 1)
		FInterval = 1;
	else
		FInterval = interval;
}

TTimer::TTimer( DWORD threadid , int interval)
{
	FThreadID = threadid;
	FMainQueue = NULL;

    InitializeCriticalSection( &FMutex);

	if (interval < 1)
		FInterval = 1;
	else
        FInterval = interval;
}

void TTimer::Active()
{
    handle = CreateThread(NULL, 56 * 1024, (LPTHREAD_START_ROUTINE)TimerCreateThread, (void*)this, 0, &pt);
    if (handle == NULL)  throw GetLastError();	
}


TTimer::~TTimer()
{
	DeleteCriticalSection(&FMutex);
}


void TTimer::AfterGet (int p1, int p2)
{
	EnterCriticalSection( &FMutex );
	
	std::list<TTimerItem>::iterator iter;
	for (iter = FList.begin (); iter != FList.end(); iter++) 
	{
		TTimerItem &CurrentMsg = (*iter);
		if (CurrentMsg.p1 == p1 && CurrentMsg.p2 == p2) 
		{
			CurrentMsg.WaitAfterGet = false;
			break;
		}
	}
    LeaveCriticalSection( &FMutex );
 }


void TTimer::Execute()
{ 
	while (1) {
		Sleep(FInterval*1000);//ÿ��Finterval��ִ��һ�Ρ�
		EnterCriticalSection( &FMutex );

		std::list<TTimerItem>::iterator iter = this->FList.begin();

		while (iter != FList.end())
		{
			TTimerItem &CurrentMsg = (*iter);
			if (CurrentMsg.WaitAfterGet)//�Ƿ�ȴ��Ժ��������ã�Ϊtrue��
				++iter;
			else 
			{
				CurrentMsg.Tick--;//�ڲ���ʱ����
				if (CurrentMsg.Tick == 0) 
				{
					TMessage *msg = new TMessage;
					msg->Type = mtTimer;
					msg->Param[0] = CurrentMsg.Count;
					msg->Param[1] = CurrentMsg.p1;
					msg->Param[2] = CurrentMsg.p2;
					msg->Param[3] = (long)this;
					if (FThreadID)
					{
						PostThreadMessage (FThreadID, WM_TIMER_MSG, (WPARAM) this, (LPARAM)msg);
					}
					else if (FMainQueue)
					{
						FMainQueue->PostMessage(msg);
					}						

					if (CurrentMsg.Onece) 
					{
						iter = FList.erase(iter);
					}						
					else 
					{
						CurrentMsg.Tick = CurrentMsg.Count;
						CurrentMsg.WaitAfterGet = true;//��ʾ�����ʱ���Ѿ���ʱ�ˣ������Ҫ����Ҫ�������á�
						++iter;
					}
				} 
				else
					++iter;
			}
		} 
        LeaveCriticalSection( &FMutex );
 	}
}


void TTimer::SetTimer(int Count, int p1, int p2, bool Onece)
{
	if ((FThreadID == 0) || !Count)
		return;
	TTimerItem Item;
	if (Count / FInterval < 1)
		Item.Count = 1;
	else
		Item.Count = Count / FInterval;
	Item.Onece = Onece;
	Item.WaitAfterGet = false;
	Item.Tick = Count / FInterval;//���õļ������С�������λ�������ֵ��һ������������С�����1��Ļ����պþͶ�Ӧס�ˡ�
	Item.p1 = p1;
	Item.p2 = p2;
	
	EnterCriticalSection( &FMutex );
	
	FList.push_back(Item);
    LeaveCriticalSection( &FMutex );
 }


void TTimer::StopTimer (int p1, int p2)
{
	EnterCriticalSection( &FMutex );
	std::list<TTimerItem>::iterator iter;
	for (iter = FList.begin (); iter != FList.end();)
	{
		TTimerItem &CurrentMsg = (*iter);
		if (CurrentMsg.p1 == p1 && CurrentMsg.p2 == p2) 
		{
			iter = FList.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
			
	}

    LeaveCriticalSection( &FMutex );
 }	


void TTimer::Clear ()
{
	EnterCriticalSection( &FMutex );

	FList.clear();

    LeaveCriticalSection( &FMutex );
}

}
