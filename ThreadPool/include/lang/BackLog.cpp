#include <Windows.h>
#include "lang/BackLog.h"
#include "lang/DateUtil.h"
#include "lang/CharSet.h"


namespace lang {


DWORD WINAPI TBackLog::BackLogCreateThread(void *owner)
{
	TBackLog *BackLog = (TBackLog *)owner;
	BackLog->Execute();
	return NULL;
}

TBackLog::TBackLog(const std::string _name, const std::string _path) :
		FDebug(false), FMaxFileSize(1024 * 1024 * 3), FPath(_path), FName(_name)
{
	FLevel = logDebug;
	if (FPath.length() == 0)
		throw lang::Exception("没有设置日志存放路径");

	OpenCurrentFile();
	handle = CreateThread(NULL, 56 * 1024, (LPTHREAD_START_ROUTINE)BackLogCreateThread, (void*)this, NULL, &pt);
	if (handle == NULL)  throw GetLastError();
	//SetThreadPriority(handle,THREAD_PRIORITY_LOWEST);
	SetThreadPriority(handle,THREAD_PRIORITY_IDLE);
	
	//FQueue = new TMessageLog(handle);//处理日志的线程的句柄。
	FQueue = new TMessageLog();
}

TBackLog::~TBackLog()
{
	if (FQueue != NULL)
	{
		if (FQueue->GetMessageA()!= NULL)
		{
			ResumeThread(handle);
			Sleep(200);
		}
		else
		{
			DWORD code = 0;
			TerminateThread(handle,code);
		}
	}	 
	outFile.close();
	delete FQueue;
}

std::string TBackLog::GetFileName(int _day, int _index)
{
	std::string _filename = TCharSet::Format("%s\\%s_%d_%d.LOG",FPath.c_str(),FName.c_str(),_day%100,_index);
	return _filename;
}

int TBackLog::GetDateFromFile(const std::string &_filename)
{
	std::ifstream in(_filename.c_str());
	if (in == NULL) return -1;

	std::string str;
	in >> str;
	if (str.length() != 10) {
		in.close();
		return -1;
	}

	str.replace(4, 1, "");
	str.replace(6, 1, "");
	in.close();
	return atoi(str.c_str());
}

void TBackLog::OpenCurrentFile()
{
	FToday = TDateUtil::GetLocalDate();

	//得到当前使用的文件Index
	int count = 0;
	std::string _filename;
	while(1)
	{
		_filename = GetFileName(FToday, count);
		std::ifstream in(_filename.c_str());
		if (in.is_open() == false)
			break;
		else
		{
			count ++;
			in.close();
		}
	}

	FFileIndex = (count == 0 ? 0 : count - 1);
	_filename = GetFileName(FToday, FFileIndex);

	//当天已经写过的文件
	if (GetDateFromFile(_filename) == FToday)
		outFile.open(_filename.c_str(), std::ios::app);
	else
		outFile.open(_filename.c_str(), std::ios::trunc);
	if (outFile.fail())
	{
		int _err = errno;
		std::string _etext = TCharSet::Format("%s打开失败[%d]",_filename.c_str(),_err);
		throw lang::Exception(_etext);
	}

	std::string todaystr = TDateUtil::FormatDateStr(FToday, '-');
	outFile << todaystr << std::string("\n");
	outFile.flush();
}

int TBackLog::WriteLog(std::string *data)
{
	if (outFile.is_open() == false)
		return -1;
	int nowday = TDateUtil::GetLocalDate();
	int _err = 0;

	outFile << (*data) << std::string("\n");
	if (outFile.fail())
	{
		_err = errno;
		return _err;
	}

	//检查有没有隔天和有没有超过大小限制
	if (FToday != nowday || outFile.tellp() >= FMaxFileSize)
	{
		if (FToday == nowday)
			FFileIndex++;
		else
		{
			FToday = nowday;
			FFileIndex = 0;
		}
		outFile.close();

		std::string _file = GetFileName(FToday, FFileIndex);
		outFile.open(_file.c_str(), std::ios::trunc);
		if (outFile.fail())
		{
			_err = errno;
			return _err;
		}
		outFile.close();
		outFile.open(_file.c_str(), std::ios::app);

		std::string _date = TDateUtil::FormatDateStr(FToday, '-');
		outFile << _date << std::string("\n");
		if (outFile.fail())
		{
			_err = errno;
			return _err;
		}
	}
	outFile.flush();
	return _err;
}

static void ClearBuffer(void *message)
{
	std::string *data = (std::string *)message;
	delete data;
}

void TBackLog::Execute()
{
	std::string* pString = NULL;
	while (1)
	{
		pString = FQueue->GetMessage();
		if (pString != NULL)
		{
			try
			{
				WriteLog(pString);
			}
			catch(...){}
			ClearBuffer(pString);
		}
	}

}

void TBackLog::AddLog(int level, const char *fmt, ...)
{
	if (FLevel < level)
		return;
	char cp[1024 + 20];
	int nowtm = TDateUtil::GetLocalTime();
	sprintf_s(cp, "%02d:%02d:%02d[%d] ", nowtm/10000, (nowtm/100)%100, nowtm%100, level);
	//sprintf(cp, "%02d:%02d:%02d[%d] ", nowtm/10000, (nowtm/100)%100, nowtm%100, level);
	va_list ap;
	va_start(ap, fmt);
	//char * _DstBuf, _In_ size_t _SizeInBytes, _In_ size_t _MaxCount, _In_z_ _Printf_format_string_ const char * _Format, va_list _ArgList);	

	//_vsnprintf_s(cp,12,1024,fmt,ap);
	_vsnprintf(cp + 12, 1024, fmt, ap);
	va_end(ap);
	cp[1024 + 20 - 1] = '\0';
	std::string *data = new std::string(cp);
	FQueue->PostMessage(data);
}

void TBackLog::AddLog(int level, const std::string &text)
{
	if (FLevel < level)
		return;
	std::string str = TDateUtil::GetLocalTimeStr();
	str += lang::TCharSet::Format("[%d]",level);
	std::string *data = new std::string(str);
	(*data).append(" ").append(text);
	FQueue->PostMessage(data);
}

}
