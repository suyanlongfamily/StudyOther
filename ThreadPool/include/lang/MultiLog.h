/*!
 * Copyright (c) 2003-2010 ERAYT Software Inc.
 * All rights reserved.

 * ����־��¼��
 * ���̲߳���ȫ�ġ�
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
			����˵�����������������
			����˵����
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
			������־��Ŀ
		*/
		void SetLogger(int type,lang::TBackLog *logger,bool isError=false)
		{
			TLogItem* item = new TLogItem;//����ط��ѵ�û�д�
			item->type = type;
			item->logger = logger;
			this->add(item);
			if (isError)//������־���͡�
				FErrLogger = logger;//
		}

		/*!
			������־����
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
			�����Ƿ����ģʽ������Ļ��ӡ���
		*/
		void SetDebug(bool debug)
		{
			FDebug = debug;
		}

		/*!
			��¼��־����,type��ʾ����ʲô��־���͵��ļ����棬level������־��ʾʲôˮƽ��
			//enum { logError = 1, logNormal, logDebug, logXnet };
		*/
		bool AddLog(int type,int level,std::string txt)
		{
			iterator iter;
			for (iter = begin(); iter != this->end(); iter++)
			{
				if ((*iter)->type == type)//��־���ͻ��֡�
				{
					(*iter)->logger->AddLog(level,txt);
					if (level == lang::TBackLog::logError && FErrLogger != NULL)
					{
						//���󼶱�����ȫ����ӡ��ͬһ���ĵ����档
						FErrLogger->AddLog(level,txt);
					} 	
					if (FDebug)//�����Ƿ���DOS��Ļ��ӡ������Ϊtrue ��cout��ӡ��
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
			��¼��־����
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
			��ȡ��־ָ��
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
		lang::TBackLog *FErrLogger;	//���д��󼶱�Ĵ���
		bool FDebug;//�����Ƿ���DOS��Ļ��ӡ������Ϊtrue ��cout��ӡ��
		int FLevel;
	};
}
#endif

