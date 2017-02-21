// 线程池.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FThreadPool.h"
#include <iostream>
#include <string>
using namespace std;

void fun(void* p)
{
	cout<<"线程池！"<<endl;

}


class Test
{
public:
	static void fun()
	{
		cout<<"---"<<endl;
	}
protected:
private:
};


void DealDataFun(
	__out LPDWORD lpNumberOfBytesTransferred,
	__out PULONG_PTR lpCompletionKey,
	__out LPOVERLAPPED *lpOverlapped
	)
{

	//处理xnetd发送过来数据的主要线程。即主处理类
	int p  = 0; //(int*)lpCompletionKey;
	//指针的强大。
	memcpy(&p,(void*)(*lpCompletionKey),sizeof(int));

	while (p)
	{
		cout<<"----过几天，我就要走了过几天，我就要走了过几天，我就要走了过几天，我就要走了过几天，我就要走了！"<<p<<endl;
		p--;	
	} 
}

int _tmain(int argc, _TCHAR* argv[])
{
	//ThreadPool tp;
	//for (int i=0;i<200;i++)
	//{
	//	tp.Call(fun,0);
	//}
	//Test t;

	FThreadPool* threadPool = FThreadPool::GetTreadPool();

	threadPool->StartTreadPool(DealDataFun,NULL);
	int* p = new int(9000);
	threadPool->PostTaskToQueue(0,(ULONG_PTR)p);  
	int* pp = new int(9000);
	threadPool->PostTaskToQueue(0,(ULONG_PTR)pp);


	//threadPool.StartTreadPool();

	std::system("pause");
	return 0;
}

