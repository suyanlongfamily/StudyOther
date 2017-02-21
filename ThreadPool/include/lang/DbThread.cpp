#include "lang/DateUtil.h"
#include "lang/CharSet.h"
#include "oracle/DriverManager.h"
#include "oracle/ResultSet.h"
#include "lang/DbThread.h"

namespace lang {

//------------------------------------------------------------------------------
/*
    线程调用
 */
DWORD WINAPI TDbConnThread::ActionCreateThread(void *owner)
{
    TDbConnThread * Action = (TDbConnThread *)owner;
    Action->Execute();
    return NULL;
}
void TDbConnThread::AddTask(std::string &str)
{
	FMainQueue->PostMessage(str);
}

void TDbConnThread::AddTask(TSQLList &sqllist)
{
	TSQLList::iterator iter;
	for (iter = sqllist.begin(); iter != sqllist.end(); iter++)
		FMainQueue->PostMessage((*iter));
}
//------------------------------------------------------------------------------
/*
	线程处理主函数
 */
void TDbConnThread::Execute()
{
	oracle::ResultSet rs( oracle::DriverManager::getConnection(FSession) );
    std::list <std::string> *mlist;
    while( (mlist = FMainQueue->GetList()) != NULL)
    {
 		while(!mlist->empty())
		{
			std::string msg = mlist->front();
			try
			{
				if (FLogger)
					FLogger->AddLog(lang::TBackLog::logDebug,"执行sql[%s]",msg.c_str());
				rs.executeUpdate(msg);
			}	
			catch(oracle::SQLException &e)
			{
				if (FLogger)
					FLogger->AddLog(lang::TBackLog::logError,"执行sql[%s]错误[%d]:%s",e.sql(),e.sqlcode(),e.what());
				std::cout << "执行sql错误:" << e.sql() << " " << e.sqlcode() << std::endl;
			}
			catch(...)
			{
				if (FLogger)
					FLogger->AddLog(lang::TBackLog::logError,"执行sql[%s]未知错误",msg.c_str());
				std::cout << "执行sql错误:" << msg << std::endl;
			}
			mlist->pop_front();
		}

        //ADD BY ZHAOGAOPAN 20071220
        delete mlist;
		mlist = NULL;
	}
	rs.close();

}
//------------------------------------------------------------------------------
/*!
	构造处理类
 */
TDbConnThread::TDbConnThread(int session,lang::TBackLog *logger)
{
    handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActionCreateThread, (void*)this, 0, &pt);
    if (handle == NULL)  throw GetLastError();
	FMainQueue = new TMessageList(handle);

    FSession = session;
	FLogger = logger;

    if (FLogger)
        FLogger->AddLog(lang::TBackLog::logNormal,"DBThread线程启动成功,数据库连接ID[%d]",FSession);
}
//------------------------------------------------------------------------------
TDbConnThread::~TDbConnThread()
{
	delete FMainQueue;
    if (handle)
        CloseHandle(handle);
}
//-----------------------------------------------------------------------------
}	//namespace base
