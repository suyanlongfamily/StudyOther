#include "StdAfx.h"
#include "FThreadPool.h"


FThreadPool* FThreadPool::threaPool = NULL;
FThreadPool* FThreadPool::GetTreadPool(){
	if (threaPool == NULL)
	{
		threaPool = new FThreadPool();
	}
	return threaPool;
}

FThreadPool::FThreadPool()
{  	
	
	SYSTEM_INFO si;   
	GetSystemInfo(&si); 
	cpuNumber = si.dwNumberOfProcessors;//获取CPU进程数 
	threadHandle.clear();
	callBack = NULL;
	threaPool = NULL;
	IOCPPort = NULL;
	IOCPPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0); 	
}

DWORD WINAPI FThreadPool::ThreadFun(LPVOID lpThreadParameter)
{
	FThreadPool* This = (FThreadPool*)lpThreadParameter;
	This->_ThreadFun(lpThreadParameter);
	return 0;
}

void FThreadPool::_ThreadFun(void* param)
{	

	DWORD       dwSize = 0;
	ULONG_PTR	pCltHandle = 0;	//带出参数。即引用。memcpy形式。
	OVERLAPPED  *pOverlapped = 0; 	 
	while (true)
	{
		BOOL ret = GetQueuedCompletionStatus(IOCPPort,&dwSize, (ULONG_PTR *)&pCltHandle, &pOverlapped, INFINITE);
		DWORD dwerror = GetLastError();		
		if (callBack != NULL)
		{
			this->callBack(&dwSize,&pCltHandle, &pOverlapped);
		}	 		
	}
} 

//************************************
// Method:    StartTreadPool
// FullName:  FThreadPool::StartTreadPool
// Access:    public 
// Returns:   void
// Qualifier: //启动线程池
// Parameter: LPTHREAD_START_ROUTINE threadFunction 线程池函数
// Parameter: void * param 对应线程池函数
//************************************
void FThreadPool::StartTreadPool(FThreadPool::CallBack _callback,void* param)
{
	callBack = _callback; 
	for (int i = 0; i < 2*cpuNumber; ++i)
	{
		threadHandle.push_back(CreateThread(NULL,0,ThreadFun,this,0,0));
	}
}


//得到IOCP端口
void* FThreadPool::GetIOCPPortHandle()
{
	return IOCPPort;
}

//投递任务到线程池 
//实际应用就 dwCompletionKey
BOOL FThreadPool::PostTaskToQueue(DWORD dwNumberOfBytesTransferred,ULONG_PTR dwCompletionKey,LPOVERLAPPED lpOverlapped)
{ 
//__in      DWORD dwNumberOfBytesTransferred,   
//__in      ULONG_PTR dwCompletionKey,  //整形值 
//__in_opt  LPOVERLAPPED lpOverlapped  
	BOOL ret = PostQueuedCompletionStatus(IOCPPort,dwNumberOfBytesTransferred,dwCompletionKey,lpOverlapped);
	return ret;	
}

/*!
 *2016/06/20 12:34:47
 *\author suyanlong
 * 更新回调函数
 */
void FThreadPool::UpdateCallBack(CallBack _callback,void* param)
{
	callBack = _callback;
}

/*!
 *2016/06/20 12:35:04
 *\author suyanlong
 * 析构 释放资源
 */
FThreadPool::~FThreadPool(void)
{
	CloseHandle(IOCPPort);
	for (int i = 0;i < cpuNumber ;++i)
	{
		if (threadHandle[i] != 0)
		{
			CloseHandle(threadHandle[i]);
		} 		
	}	   
}


AsyncThreadPool::AsyncThreadPool()
{
	FcallBack = NULL;
}

int AsyncThreadPool::PostTaskToThreadPool(void* pvContext)
{
	//BOOL TrySubmitThreadpoolCallback(
	//	PTP_SIMPLE_CALLBACK pfnCallback,//异步函数的指针
	//	PVOID pvContext,          //传递给异步函数的参数
	//	PTP_CALLBACK_ENVIRON pcbe);

	int ret = 0;
	if (FcallBack != NULL)
	{
		ret = TrySubmitThreadpoolCallback(FcallBack,pvContext,NULL);
	}
	return ret;  
}  

void AsyncThreadPool::SetCallBack(CallBack callBack)
{
	FcallBack = callBack;
}
