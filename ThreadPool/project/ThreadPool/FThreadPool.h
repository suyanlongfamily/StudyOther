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
	
	//回调函数类型 定义式
	typedef void (* CallBack)(
		__out LPDWORD lpNumberOfBytesTransferred,
		__out PULONG_PTR lpCompletionKey,
		__out LPOVERLAPPED *lpOverlapped
	); 
	~FThreadPool(void);

	/*!
	 *2016/06/19 20:05:08
	 *\author suyanlong
	 *  单例模式
	 */
	static FThreadPool* GetTreadPool();

	//启动线程池
	void StartTreadPool(FThreadPool::CallBack _callback,void* param);

	//得到IOCP端口
	void* GetIOCPPortHandle();

	//投递任务到线程池	  
	BOOL PostTaskToQueue(DWORD dwNumberOfBytesTransferred,ULONG_PTR dwCompletionKey,LPOVERLAPPED lpOverlapped = NULL);

	void UpdateCallBack(CallBack _callback,void* param = NULL);	

	//void DealDataFun(
	//	__out LPDWORD lpNumberOfBytesTransferred,
	//	__out PULONG_PTR lpCompletionKey,
	//	__out LPOVERLAPPED *lpOverlapped
	//	);

private:
	CallBack callBack;//保存回调函数
	HANDLE IOCPPort;
	int cpuNumber;
	HANDLE threadHandle[MAXTHREADNUMBER]; //存放线程句柄的容器。
	static DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);  
	void _ThreadFun(void* param);							  
	static FThreadPool* threaPool;
	FThreadPool(void);
	FThreadPool(FThreadPool& F){}
	FThreadPool& operator= (FThreadPool F){} 	
};




#endif


