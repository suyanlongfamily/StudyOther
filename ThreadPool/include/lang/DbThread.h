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
 * @(#)DbThread.h
 *
 */
//---------------------------------------------------------------------------

#ifndef _LANG_DBTHREAD_H
#define _LANG_DBTHREAD_H

#include "lang/NewList.h"
#include "lang/BackLog.h"
#include "xnet/ThreadList.h"

namespace lang {

typedef lang::List<std::string> TSQLList;
class TDbConnThread
{
public:
	TDbConnThread(int session,lang::TBackLog *logger);

	~TDbConnThread();

	// 线程处理主函数
	void Execute();

	// 接口
	void AddTask(std::string &str);
	
	void AddTask(TSQLList &sqllist);
private:
    static DWORD WINAPI ActionCreateThread(void *owner);
	// 重命名线程消息队列
	typedef xnet::TemplateMessageList<std::string> TMessageList;
	TMessageList *FMainQueue;
	HANDLE handle;
    DWORD pt;
    int FSession;
	lang::TBackLog *FLogger;
};

}	//namespace base
#endif
