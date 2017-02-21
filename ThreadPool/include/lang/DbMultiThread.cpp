#include "lang/DateUtil.h"
#include "lang/CharSet.h"
#include "oracle/DriverManager.h"
#include "oracle/ResultSet.h"

#include "lang/DbMultiThread.h"

namespace lang {

//------------------------------------------------------------------------------
/*
    �̵߳���
 */
DWORD WINAPI TDbMultiThread::ActionCreateThread(void *owner)
{
    TDbMultiThread * Action = (TDbMultiThread *)owner;
    Action->Execute();
    return NULL;
}
void TDbMultiThread::AddTask(std::string &str)
{
    TSQLlist *sqllist = new TSQLlist;
    sqllist->push_back(str);
	FMainQueue->PostMessage(sqllist);
}

void TDbMultiThread::AddTask(TSQLlist &sqllist)
{
    TSQLlist *list = new TSQLlist(sqllist);
    FMainQueue->PostMessage(list);
}
//------------------------------------------------------------------------------
/*
	�̴߳���������
 */
void TDbMultiThread::Execute()
{
	oracle::ResultSet rs( oracle::DriverManager::getConnection(FSession) );
    TSQLlist *mlist;
    while(1)
    {
        mlist = NULL;
        try
        {
            mlist = FMainQueue->GetMessage();
            if (FLogger)
                FLogger->AddLog(lang::TBackLog::logNormal,"ִ��sql,����[%d]",mlist->size());
            TSQLlist::iterator iter;
            for (iter = mlist->begin(); iter != mlist->end(); iter++)
            {
                if (FLogger)
                    FLogger->AddLog(lang::TBackLog::logNormal,"ִ��sql[%s]",(*iter).c_str());
            }
            rs.executeUpdate(*mlist);
        }
        catch(oracle::SQLException &e)
        {
            if (FLogger)
                FLogger->AddLog(lang::TBackLog::logError,"ִ��sql[%s]����[%d]:%s",e.sql(),e.sqlcode(),e.what());
            std::cout << "ִ��sql����:" << e.sql() << " " << e.sqlcode() << std::endl;
            TSQLlist::iterator iter;
            for (iter = mlist->begin(); iter != mlist->end(); iter++)
            {
                if (FLogger)
                    FLogger->AddLog(lang::TBackLog::logError,"�ع�sql[%s]",(*iter).c_str());
            }
        }
        catch(...)
        {
            if (FLogger)
                FLogger->AddLog(lang::TBackLog::logError,"ִ��sqlδ֪����");
            std::cout << "ִ��sqlδ֪����" << std::endl;
            TSQLlist::iterator iter;
            for (iter = mlist->begin(); iter != mlist->end(); iter++)
            {
                if (FLogger)
                    FLogger->AddLog(lang::TBackLog::logError,"�ع�sql[%s]",(*iter).c_str());
            }
        }
        if (mlist)
            delete mlist;
    }
	rs.close();
}
//------------------------------------------------------------------------------
/*!
	���촦����
 */
TDbMultiThread::TDbMultiThread(int session,lang::TBackLog *logger,TMessageList *queue)
{
    handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ActionCreateThread, (void*)this, 0, &pt);
    if (handle == NULL)  throw GetLastError();
	FMainQueue = queue;

    FSession = session;
	FLogger = logger;

    if (FLogger)
        FLogger->AddLog(lang::TBackLog::logNormal,"DBThread�߳������ɹ�,���ݿ�����ID[%d]",FSession);
}
//------------------------------------------------------------------------------
TDbMultiThread::~TDbMultiThread()
{
    if (handle)
        CloseHandle(handle);
}
//-----------------------------------------------------------------------------
}	//namespace base
