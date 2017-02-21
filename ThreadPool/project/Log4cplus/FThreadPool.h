#ifndef FTREADPOOL_H
#define FTREADPOOL_H
#pragma once  

/*  
 *	���ߣ�������		
 * 	ʱ�䣺2016/6/25
 *
 *	�̳߳�ʹ�ó�����
 *	1��Ͷ�ݹ��������첽ִ��һ���ص���������������ͨ���ص�������������ص������ڲ���
 *	2������windowsƽ̨��IOCP��
 *	3��Ͷ������ʱ������ͨ��ָ��ķ�ʽ����Լ������
 *	�ܽ᣺����һ���̳߳�ģ�ͣ�Ͷ�������̳߳أ�ͬ����ʹ���߿��Ž��������߼�����ʹ����ע�ᣨ�ص������������߳�ִ�к�����
 */

#include <Windows.h>  
#include <string>
#include <vector>
using namespace std;  

class FThreadPool
{
public:
							     
	//�ص��������� ����ʽ windows��"__out" ���������꣬��ʾ����׼�����ͱ���,ͨ����ַ������ 
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
	BOOL PostTaskToQueue(
				__in DWORD dwNumberOfBytesTransferred,    
				__in ULONG_PTR dwCompletionKey,   
				__in_opt LPOVERLAPPED lpOverlapped = NULL
						);

	//���»ص�����
	void UpdateCallBack(CallBack _callback,void* param = NULL);	

private:
	CallBack callBack;//����ص�����
	HANDLE IOCPPort;
	int cpuNumber;
	std::vector<HANDLE> threadHandle;//����߳̾���������� 
	static DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);  
	void _ThreadFun(void* param);							  
	static FThreadPool* threaPool;
	FThreadPool(void);
	FThreadPool(FThreadPool& F){}
	FThreadPool& operator= (FThreadPool& F){} 	
}; 


 /*  
 *	���ߣ�������		
 * 	ʱ�䣺2016/6/25
 *
 *	�̳߳�ʹ�ó�����
 *	1��Ͷ�ݹ��������첽ִ��һ���ص���������������ͨ���ص�������������ص������ڲ���
 *	2��windows �Դ����̳߳ء�
 *	3��Ͷ������ʱ������ͨ��ָ��ķ�ʽ����Լ������
 *	4��ʹ�����ƣ��ڴ�����㹻�󣬵��ڴ��Сʱ��Ͷ������ʧ�ܡ�
 *	�ܽ᣺����һ���̳߳�ģ�ͣ�Ͷ�������̳߳أ�ͬ����ʹ���߿��Ž��������߼�����ʹ����ע�ᣨ�ص������������߳�ִ�к�����
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
	 *	ע��ص�������
	 */
	void SetCallBack(CallBack callBack); 

	/*!
	 *2016/06/25 22:19:44
	 *\author suyanlong
	 *	1��Ͷ������
	 *  2��pvContext ���ݸ�ע��Ļص������Ĳ�����
	 *  3��return 0 ��ʾͶ������ʧ�ܡ�
	 */	 	
	int PostTaskToThreadPool(void* pvContext);	

	~AsyncThreadPool(){};
	AsyncThreadPool(); 
protected: 
private:  
	CallBack FcallBack;	//�ص����� ����Լ�� _stdcall 
};	


#endif


