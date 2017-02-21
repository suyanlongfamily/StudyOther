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
	cpuNumber = si.dwNumberOfProcessors;//��ȡCPU������ 
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
	ULONG_PTR	pCltHandle = 0;	//���������������á�memcpy��ʽ��
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
// Qualifier: //�����̳߳�
// Parameter: LPTHREAD_START_ROUTINE threadFunction �̳߳غ���
// Parameter: void * param ��Ӧ�̳߳غ���
//************************************
void FThreadPool::StartTreadPool(FThreadPool::CallBack _callback,void* param)
{
	callBack = _callback; 
	for (int i = 0; i < 2*cpuNumber; ++i)
	{
		threadHandle.push_back(CreateThread(NULL,0,ThreadFun,this,0,0));
	}
}


//�õ�IOCP�˿�
void* FThreadPool::GetIOCPPortHandle()
{
	return IOCPPort;
}

//Ͷ�������̳߳� 
//ʵ��Ӧ�þ� dwCompletionKey
BOOL FThreadPool::PostTaskToQueue(DWORD dwNumberOfBytesTransferred,ULONG_PTR dwCompletionKey,LPOVERLAPPED lpOverlapped)
{ 
//__in      DWORD dwNumberOfBytesTransferred,   
//__in      ULONG_PTR dwCompletionKey,  //����ֵ 
//__in_opt  LPOVERLAPPED lpOverlapped  
	BOOL ret = PostQueuedCompletionStatus(IOCPPort,dwNumberOfBytesTransferred,dwCompletionKey,lpOverlapped);
	return ret;	
}

/*!
 *2016/06/20 12:34:47
 *\author suyanlong
 * ���»ص�����
 */
void FThreadPool::UpdateCallBack(CallBack _callback,void* param)
{
	callBack = _callback;
}

/*!
 *2016/06/20 12:35:04
 *\author suyanlong
 * ���� �ͷ���Դ
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
	//	PTP_SIMPLE_CALLBACK pfnCallback,//�첽������ָ��
	//	PVOID pvContext,          //���ݸ��첽�����Ĳ���
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
