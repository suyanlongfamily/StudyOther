#ifndef FTREADPOOL_H
#define FTREADPOOL_H
#pragma once  

/*  
 *	作者：苏彦龙		
 * 	时间：2016/6/25
 *
 *	线程池使用场景：
 *	1、投递工作任务，异步执行一个回调函数，工作任务通过回调函数参数带入回调函数内部。
 *	2、机制windows平台的IOCP。
 *	3、投递任务时，建议通过指针的方式，节约开销。
 *	总结：开放一个线程池模型，投递任务到线程池，同样对使用者开放解决任务的逻辑，由使用者注册（回调函数），即线程执行函数。
 */

#include <Windows.h>  
#include <string>
#include <vector>
using namespace std;  

class FThreadPool
{
public:
							     
	//回调函数类型 定义式 windows的"__out" 带出参数宏，表示事先准备类型变量,通过地址带出。 
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
	BOOL PostTaskToQueue(
				__in DWORD dwNumberOfBytesTransferred,    
				__in ULONG_PTR dwCompletionKey,   
				__in_opt LPOVERLAPPED lpOverlapped = NULL
						);

	//更新回调函数
	void UpdateCallBack(CallBack _callback,void* param = NULL);	

private:
	CallBack callBack;//保存回调函数
	HANDLE IOCPPort;
	int cpuNumber;
	std::vector<HANDLE> threadHandle;//存放线程句柄的容器。 
	static DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);  
	void _ThreadFun(void* param);							  
	static FThreadPool* threaPool;
	FThreadPool(void);
	FThreadPool(FThreadPool& F){}
	FThreadPool& operator= (FThreadPool& F){} 	
}; 


 /*  
 *	作者：苏彦龙		
 * 	时间：2016/6/25
 *
 *	线程池使用场景：
 *	1、投递工作任务，异步执行一个回调函数，工作任务通过回调函数参数带入回调函数内部。
 *	2、windows 自带的线程池。
 *	3、投递任务时，建议通过指针的方式，节约开销。
 *	4、使用限制：内存必须足够大，当内存很小时，投递任务失败。
 *	总结：开放一个线程池模型，投递任务到线程池，同样对使用者开放解决任务的逻辑，由使用者注册（回调函数），即线程执行函数。
 */

class AsyncThreadPool
{
public:
	typedef void (WINAPI * CallBack )(		
		PTP_CALLBACK_INSTANCE pInstance,
		PVOID pvContext);

	/*!
	 *2016/06/25 22:19:52
	 *\author suyanlong
	 *	注册回调函数。
	 */
	void SetCallBack(CallBack callBack); 

	/*!
	 *2016/06/25 22:19:44
	 *\author suyanlong
	 *	1、投递任务。
	 *  2、pvContext 传递给注册的回调函数的参数。
	 *  3、return 0 表示投递任务失败。
	 */	 	
	int PostTaskToThreadPool(void* pvContext);	

	~AsyncThreadPool(){};
	AsyncThreadPool(); 
protected: 
private:  
	CallBack FcallBack;	//回调函数 调用约定 _stdcall 
};	


#endif


