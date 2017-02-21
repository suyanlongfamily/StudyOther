/*!
 * Copyright (c) 2003-2010 ERAYT Software Inc.
 * All rights reserved.
 *
 * 写Oracle线程
 *
 *              create by weiyaowu  2007/06/27
 *              modify by zhaogaopan 2007/12/20
 *              modify by hushumin 2008/12/02
 * 函数清单:
 *
 * @(#)DbMultiThread.h
 *
 */
//---------------------------------------------------------------------------

#ifndef _LANG_DBMULTITHREAD_H
#define _LANG_DBMULTITHREAD_H

#include "lang/NewList.h"
#include "lang/BackLog.h"
#include "xnet/SafeQueue.h"
#include "lang/DbThread.h"

namespace lang {


typedef std::list<std::string> TSQLlist;

class TDbMultiThread
{
public:
	// 重命名线程消息队列
	typedef xnet::TemplateMessageQueue<TSQLlist *> TMessageList;

    TDbMultiThread(int session,lang::TBackLog *logger,TMessageList *queue);

	~TDbMultiThread();

	// 线程处理主函数
	void Execute();

	// 入口
	void AddTask(std::string &str);

    // 入口
    void AddTask(TSQLlist &sqllist);
private:
    static DWORD WINAPI ActionCreateThread(void *owner);
	TMessageList *FMainQueue;
	HANDLE handle;
    DWORD pt;
    int FSession;
	lang::TBackLog *FLogger;
};

}	//namespace base
#endif
