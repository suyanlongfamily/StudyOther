#ifndef FTREADPOOL_H
#define FTREADPOOL_H

#include <Windows.h>

#include <string>
#include <list>

using namespace std;

#define  MAXTHREADNUMBER	100
#pragma once 	 
//
//void DealDataFun(
//	__out LPDWORD lpNumberOfBytesTransferred,
//	__out PULONG_PTR lpCompletionKey,
//	__out LPOVERLAPPED *lpOverlapped
//	);

class FThreadPool
{
public:
	
	//�ص��������� ����ʽ
	typedef void (* CallBack)(
		__out LPDWORD lpNumberOfBytesTransferred,
		__out PULONG_PTR lpCompletionKey,
		__out LPOVERLAPPED *lpOverlapped
	); 
	~FThreadPool(void);

	/*!
	 *2016/06/19 20:05:08
	 *\author suyanlong
	 *  ����ģʽ
	 */
	static FThreadPool* GetTreadPool();

	//�����̳߳�
	void StartTreadPool(FThreadPool::CallBack _callback,void* param);

	//�õ�IOCP�˿�
	void* GetIOCPPortHandle();

	//Ͷ�������̳߳�	  
	BOOL PostTaskToQueue(DWORD dwNumberOfBytesTransferred,ULONG_PTR dwCompletionKey,LPOVERLAPPED lpOverlapped = NULL);

	void UpdateCallBack(CallBack _callback,void* param = NULL);	

	//void DealDataFun(
	//	__out LPDWORD lpNumberOfBytesTransferred,
	//	__out PULONG_PTR lpCompletionKey,
	//	__out LPOVERLAPPED *lpOverlapped
	//	);

private:
	CallBack callBack;//����ص�����
	HANDLE IOCPPort;
	int cpuNumber;
	HANDLE threadHandle[MAXTHREADNUMBER]; //����߳̾����������
	static DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);  
	void _ThreadFun(void* param);							  
	static FThreadPool* threaPool;
	FThreadPool(void);
	FThreadPool(FThreadPool& F){}
	FThreadPool& operator= (FThreadPool F){} 	
};




#endif


