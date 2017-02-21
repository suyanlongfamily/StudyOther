//
//#include <iostream>
//#include <log4cplus/helpers/loglog.h>
//
//using namespace std;
//using namespace log4cplus::helpers;
//
//void printMsgs() {
//	cout << "Entering printMsgs()..." << endl;
//
//	LogLog::getLogLog()->debug(LOG4CPLUS_TEXT("This is a Debug statement..."));
//	LogLog::getLogLog()->debug(
//		log4cplus::tstring(LOG4CPLUS_TEXT("This is a Debug statement...")));
//
//	LogLog::getLogLog()->warn(LOG4CPLUS_TEXT("This is a Warning..."));
//	LogLog::getLogLog()->warn(
//		log4cplus::tstring(LOG4CPLUS_TEXT("This is a Warning...")));
//
//	LogLog::getLogLog()->error(LOG4CPLUS_TEXT("This is a Error..."));
//	LogLog::getLogLog()->error(
//		log4cplus::tstring(LOG4CPLUS_TEXT("This is a Error...")));
//
//	cout << "Exiting printMsgs()..." << endl << endl;
//}
//
//
//int
//	main() {
//		log4cplus::initialize ();
//		printMsgs();
//
//		cout << "Turning on debug..." << endl;
//		LogLog::getLogLog()->setInternalDebugging(true);
//		printMsgs();
//
//		cout << "Turning on quiet mode..." << endl;
//		LogLog::getLogLog()->setQuietMode(true);
//		printMsgs();
//		system("pause");
//		return 0;
//}
//


#include "stdafx.h"
#include <memory>
#include <Windows.h>
#include <log4cplus/logger.h>
#include <log4cplus/layout.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>	
#include <log4cplus/asyncappender.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>
#include <iomanip>


using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace std;


//int _tmain(int argc, _TCHAR* argv[])
//{
//
//	//log4cplus::tcout<<"nihao \n";
//	//log4cplus::FileAppender	filelog("nihao.txt");
//	//log4cplus::SharedAppenderPtr
//	
//	
//	//BasicConfigurator config;
//	//config.configure();
//
//	//Logger logger = Logger::getInstance("main");
//	//LOG4CPLUS_WARN(logger, "Hello, World!");
//
//	SharedAppenderPtr pConsoleAppender(new ConsoleAppender());   
//
//	// 定义一个简单的Layout,并绑定到Appender   
//	std::auto_ptr<Layout> pSimpleLayout(new SimpleLayout()); 
//	pConsoleAppender->setLayout(pSimpleLayout);   
//
//	log4cplus::Logger pLogger = log4cplus::Logger::getInstance("trade");
//	pLogger.setLogLevel(log4cplus::WARN_LOG_LEVEL);
//
//	pLogger.addAppender(pConsoleAppender);
//	
//	LOG4CPLUS_WARN(pLogger,"nihao!");
//	LOG4CPLUS_WARN(pLogger,"nihao!");
//	LOG4CPLUS_WARN(pLogger,"nihao!");
//	LOG4CPLUS_WARN(pLogger,"nihao!");
//	LOG4CPLUS_WARN(pLogger,"nihao!");
//	LOG4CPLUS_WARN(pLogger,"nihao!");
//
//	getchar();
//	return 0;
//}

#include <iostream>
#include "FThreadPool.h"
using namespace std;

// 定义Logger 
Logger pTestLogger = Logger::getInstance(_T("LoggerName"));    

void DealDataFun(
	__out LPDWORD lpNumberOfBytesTransferred,
	__out PULONG_PTR lpCompletionKey,
	__out LPOVERLAPPED *lpOverlapped
	);

void WINAPI DealDataFun1(PTP_CALLBACK_INSTANCE pInstance,void* pvContext);


int _tmain(int argc, _TCHAR* argv[]) 
{ 


	// 定义1个控制台的Appender,3个文件Appender 
	SharedAppenderPtr pConsoleAppender(new ConsoleAppender()); 
	SharedAppenderPtr pFileAppender1(new FileAppender(_T("d:\\testlog1.log"))); 
	SharedAppenderPtr pFileAppender2(new FileAppender(_T("d:\\testlog2.log"))); 
	SharedAppenderPtr pFileAppender3(new FileAppender(_T("d:\\testlog3.log"))); 


	//SharedAppenderPtr pFileAppender4(new AsyncAppender(_T("d:\\testlog4.log")));
	

	// 定义一个简单的Layout,并绑定到 pFileAppender1 
	auto_ptr<Layout> pSimpleLayout(new SimpleLayout()); 
	pFileAppender1->setLayout(pSimpleLayout); 

	// 定义一个TTCLayout,并绑定到 pFileAppender2 
	auto_ptr<Layout> pTTCLayout(new TTCCLayout()); 
	pFileAppender2->setLayout(pTTCLayout); 

	// 定义一个PatternLayout,并绑定到 pFileAppender3 
	auto_ptr<Layout> pPatternLayout(new PatternLayout(_T("%d{%m/%d/%y %H:%M:%S}    - %m [%l]%n"))); 
	pFileAppender3->setLayout(pPatternLayout); 	

	// 将需要关联Logger的Appender添加到Logger上 
	pTestLogger.addAppender(pConsoleAppender); 
	pTestLogger.addAppender(pFileAppender1); 
	pTestLogger.addAppender(pFileAppender2); 
	pTestLogger.addAppender(pFileAppender3); 

	//FThreadPool* threadPool = FThreadPool::GetTreadPool();

	//threadPool->StartTreadPool(DealDataFun,NULL);
	//int* p = new int(900);
	//threadPool->PostTaskToQueue(0,(ULONG_PTR)p);  
	//int* pp = new int(900);
	//threadPool->PostTaskToQueue(0,(ULONG_PTR)pp);	

	AsyncThreadPool threadPool;
	threadPool.SetCallBack(DealDataFun1);
	int* p = new int(900);	
	threadPool.PostTaskToThreadPool(p);

	p = new int(900);  
	threadPool.PostTaskToThreadPool(p);

	p = new int(900);  
	threadPool.PostTaskToThreadPool(p);
	p = new int(900);  
	threadPool.PostTaskToThreadPool(p);
	p = new int(900);  
	threadPool.PostTaskToThreadPool(p);
	p = new int(900);  
	threadPool.PostTaskToThreadPool(p);	

	std::system("pause"); 
	return 0; 
}


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
		// 输出日志信息 		
		LOG4CPLUS_WARN(pTestLogger, "This is a <Warn> log message..."); 
		LOG4CPLUS_ERROR(pTestLogger, " 错误 message...");
		p--;	
	} 
}

void WINAPI DealDataFun1(PTP_CALLBACK_INSTANCE pInstance,void* pvContext)
{
	//处理xnetd发送过来数据的主要线程。即主处理类
	int p = *((int*)pvContext);
	while (p)
	{
		cout<<"----过几天，我就要走了过几天，我就要走了过几天，我就要走了过几天，我就要走了过几天，我就要走了！"<<p<<endl;
		// 输出日志信息 		
		LOG4CPLUS_WARN(pTestLogger, "This is a <Warn> log message..."); 
		LOG4CPLUS_ERROR(pTestLogger, " 错误 message...");
		p--;	
	} 

}