/*!
 * Copyright (c) 2003-2010 ERAYT Software Inc.
 * All rights reserved.

 * 多日志记录类
 * 多线程不安全的。
 *@author shenyouchun
 *@since 2010-06-25
 *@modify suyanlong
 *@modify 2015-12-3
 */
//---------------------------------------------------------------------------
#ifndef _MultiLog_H
#define _MultiLog_H

#include <string>
#include "lang/BackLog.h"
#include "lang/List.h"
#include "lang/DateUtil.h"
#include "CharSet.h"
//---------------------------------------------------------------------------
namespace lang
{
	struct TLogItem
	{
		int type;
		lang::TBackLog* logger;
	};
//---------------------------------------------------------------------------
	/*!
	 *  class TMultiLog
	 */
	class TMultiLog : public lang::List<TLogItem*>
	{
	public:

		/*!
			功能说明：构造和析构函数
			参数说明：
		*/
		TMultiLog()
		{
			FErrLogger = NULL;
		}
		virtual ~TMultiLog()
		{
			iterator iter;
			for (iter = begin(); iter != end(); iter++)
			{
				delete (*iter)->logger;
				delete (*iter);
			}
		}

		/*!
			设置日志项目
		*/
		void SetLogger(int type,lang::TBackLog *logger,bool isError=false)
		{
			TLogItem* item = new TLogItem;//这个地方难道没有错？
			item->type = type;
			item->logger = logger;
			this->add(item);
			if (isError)//错误日志类型。
				FErrLogger = logger;//
		}

		/*!
			设置日志级别
		*/
		void SetLevel(int level)
		{
			FLevel = level;
			iterator iter;
			for (iter = begin(); iter != this->end(); iter++)
			{
				(*iter)->logger->SetLevel(level);
			}
		}
		/*!
			设置是否调试模式（在屏幕打印与否）
		*/
		void SetDebug(bool debug)
		{
			FDebug = debug;
		}

		/*!
			记录日志内容,type表示放在什么日志类型的文件里面，level这条日志表示什么水平。
			//enum { logError = 1, logNormal, logDebug, logXnet };
		*/
		bool AddLog(int type,int level,std::string txt)
		{
			iterator iter;
			for (iter = begin(); iter != this->end(); iter++)
			{
				if ((*iter)->type == type)//日志类型划分。
				{
					(*iter)->logger->AddLog(level,txt);
					if (level == lang::TBackLog::logError && FErrLogger != NULL)
					{
						//错误级别数据全部打印到同一个文档里面。
						FErrLogger->AddLog(level,txt);
					} 	
					if (FDebug)//决定是否在DOS屏幕打印出来。为true 则cout打印。
					{
						if (level <= FLevel)
						{
							string str = lang::TDateUtil::GetLocalTimeStr();
							str += lang::TCharSet::Format("[%d] ",level);
							str += txt;
							std::cout << str << std::endl;
						}
						
					}										
					return true;
				}
			}
			return false;
		}
		/*!
			记录日志内容
		*/
		bool AddLog(int type,int level,const char *fmt,...)
		{
			char cp[1024 + 1];
			va_list ap;
			va_start(ap, fmt);
			_vsnprintf(cp, 1024, fmt, ap);
			va_end(ap);
			cp[1024] = '\0';
			std::string data = std::string(cp);
			iterator iter;
			for (iter = begin(); iter != this->end(); ++iter)
			{
				
				if ((*iter)->type == type)
				{
					(*iter)->logger->AddLog(level,data);
					if (level == lang::TBackLog::logError && FErrLogger != NULL)
					{
						FErrLogger->AddLog(level,data);	
					} 						

					if (FDebug)
					{
						if (level <= FLevel)
						{
							string str = lang::TDateUtil::GetLocalTimeStr();
							str += lang::TCharSet::Format("[%d] ",level);
							str += data;
							std::cout << str << std::endl;
						}
						
					}	
					return true;
				}
			}
			return false;
		}
		/*!
			获取日志指针
		*/
		lang::TBackLog *GetLogger(int type)
		{
			iterator iter;
			for (iter = begin(); iter != end(); iter++)
			{
				if ((*iter)->type == type)
					return (*iter)->logger;
			}
			return NULL;
		}
	private:
		lang::TBackLog *FErrLogger;	//所有错误级别的错误。
		bool FDebug;//决定是否在DOS屏幕打印出来。为true 则cout打印。
		int FLevel;
	};
}
#endif

