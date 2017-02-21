#include "lang/DateUtil.h"
#include "lang/CharSet.h"
#include "oracle/DriverManager.h"
#include "oracle/ResultSet.h"
#include "lang/DbThread.h"

namespace lang {

//------------------------------------------------------------------------------
/*
    �̵߳���
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
	�̴߳���������
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
					FLogger->AddLog(lang::TBackLog::logDebug,"ִ��sql[%s]",msg.c_str());
				rs.executeUpdate(msg);
			}	
			catch(oracle::SQLException &e)
			{
				if (FLogger)
					FLogger->AddLog(lang::TBackLog::logError,"ִ��sql[%s]����[%d]:%s",e.sql(),e.sqlcode(),e.what());
				std::cout << "ִ��sql����:" << e.sql() << " " << e.sqlcode() << std::endl;
			}
			catch(...)
			{
				if (FLogger)
					FLogger->AddLog(lang::TBackLog::logError,"ִ��sql[%s]δ֪����",msg.c_str());
				std::cout << "ִ��sql����:" << msg << std::endl;
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
	���촦����
 */
TDbConnThread::TDbConnThread(int session,lang::TBackLog *logger)
{
    handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActionCreateThread, (void*)this, 0, &pt);
    if (handle == NULL)  throw GetLastError();
	FMainQueue = new TMessageList(handle);

    FSession = session;
	FLogger = logger;

    if (FLogger)
        FLogger->AddLog(lang::TBackLog::logNormal,"DBThread�߳������ɹ�,���ݿ�����ID[%d]",FSession);
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
