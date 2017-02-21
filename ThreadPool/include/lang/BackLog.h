#ifndef _LANG_BACKLOG_H
#define _LANG_BACKLOG_H

#include <string>
#include <iostream>
#include <fstream>

#include "lang/SafeQueue.h"
using namespace lang;

/*!
	��¼��־����,��װ��һ�߳�ר�ż�¼������Ϣ
	-	�������̰߳�ȫ��,�����ڶ��߳���ʹ��
*/

namespace lang {

class TBackLog
{
public:
	enum { logError = 1, logNormal, logDebug, logXnet };

	/*!
		����:����Ӧ�ó��������������
		-	���������·��, Ӧ�ó������������ļ�
		-	�������:_nameӦ�ó�����, _path��־�ļ����·��
	*/
	TBackLog(const std::string _name, const std::string _path);
	/*!
		����:������־����
		-	һ��Ҫ��֤��־��������,����ᶪ��¼
		-	�������:��
	*/
	virtual ~TBackLog();

	/*!
		����:�߳�ִ���庯��
		-	��Ȼ�����ṩ�ú���,����Ա����ֱ�ӵ��øú���
	*/
	void Execute();

	/*!
		����:������־�ļ���С
		-	Ĭ���ļ���СΪ1024*1024*3,�����ļ���С�Զ���һ
		-	�������: filesize ��־�ļ���С
	*/
	void SetFileSize(unsigned int filesize) { FMaxFileSize = filesize; }

	/*!
		���õ��Լ���
	*/
	void SetLevel(int level) { FLevel = (level <= 0 || level > 3 ? 3 : level); }

	/*!
		����:�����ṩ�ļ�¼��־����
		-	��־����,��ΪlogError,logNormal,logDebug�������� д��������printf����
		-	Level=3ȫ����, Level=2ֻ��Error��Normal, Level=1ֻ��Error
	*/
	void AddLog(int level, const char *fmt, ...);

	/*!
		����string����
	*/
	void AddLog(int level, const std::string &text);

private:
	typedef lang::TemplateMessageQueue<std::string *> TMessageLog;
	//typedef xnet::TemplateMessageList<std::string *> TMessageLog;
	TMessageLog *FQueue;
	bool	FDebug;

    static DWORD WINAPI BackLogCreateThread(void *owner);

	int	FToday, FFileIndex, FLevel;
	unsigned int FMaxFileSize;
	std::string FPath, FName;
    DWORD pt;
 	HANDLE handle; //handleΪ�����̵߳ľ����

	std::ofstream outFile;
private:
	void OpenCurrentFile();
	int WriteLog(std::string *data);

	int GetDateFromFile(const std::string &_filename);
	std::string GetFileName(int _day, int _index);
};

}
#endif
