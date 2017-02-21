#ifndef _LANG_BACKLOG_H
#define _LANG_BACKLOG_H

#include <string>
#include <iostream>
#include <fstream>

#include "lang/SafeQueue.h"
using namespace lang;

/*!
	记录日志对象,封装了一线程专门记录调试信息
	-	该类是线程安全类,可以在多线程中使用
*/

namespace lang {

class TBackLog
{
public:
	enum { logError = 1, logNormal, logDebug, logXnet };

	/*!
		描述:根据应用程序名来构造对象
		-	对象类根据路径, 应用程序名来生成文件
		-	输入参数:_name应用程序名, _path日志文件存放路径
	*/
	TBackLog(const std::string _name, const std::string _path);
	/*!
		描述:析构日志对象
		-	一定要保证日志对象被析构,否则会丢记录
		-	输入参数:无
	*/
	virtual ~TBackLog();

	/*!
		描述:线程执行体函数
		-	虽然对外提供该函数,程序员不能直接调用该函数
	*/
	void Execute();

	/*!
		描述:设置日志文件大小
		-	默认文件大小为1024*1024*3,超过文件大小自动加一
		-	输入参数: filesize 日志文件大小
	*/
	void SetFileSize(unsigned int filesize) { FMaxFileSize = filesize; }

	/*!
		设置调试级别
	*/
	void SetLevel(int level) { FLevel = (level <= 0 || level > 3 ? 3 : level); }

	/*!
		描述:对外提供的记录日志函数
		-	日志级别,分为logError,logNormal,logDebug三个级别 写法类似于printf函数
		-	Level=3全部记, Level=2只记Error和Normal, Level=1只记Error
	*/
	void AddLog(int level, const char *fmt, ...);

	/*!
		输入string类型
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
 	HANDLE handle; //handle为处理线程的句柄。

	std::ofstream outFile;
private:
	void OpenCurrentFile();
	int WriteLog(std::string *data);

	int GetDateFromFile(const std::string &_filename);
	std::string GetFileName(int _day, int _index);
};

}
#endif
