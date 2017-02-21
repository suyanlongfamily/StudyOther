#ifndef _LANG_DATEUTIL_H
#define _LANG_DATEUTIL_H

#include <time.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

namespace lang {

class TDateUtil
{
public:

	/*!
		判断是否是有效日期 yyyymmdd
	*/
	static bool ValidDate(int indate);

	/*!
		判断是否是有效时间 hhmmss
	*/
	static bool ValidTime(int intime);

	/*!
		判断是否是闰年
	*/
	static bool IsLeapYear(int year);

	/*!
		返回一个月中的最大天数
	*/
	static int MaxDay(int year, int month);

	/*!
		返回从1970开始的净天数
	*/
	static int NetDayFrom1970(int indate);

	/*!
		返回两个日期之间相差的天数
	*/
	static int BetweenDays(int startdate, int enddate);

    /*!
		返回两个时间之间相差的秒数
	*/
    static int BetweenTimes(int first,int second);

	/*!
		增加天数
	*/
	static int AddDay(int indate, int day_offset);

	/*!
		返回当前日期
	*/
	static int GetLocalDate();

	/*!
		返回当前时间
	*/
	static int GetLocalTime();

	/*!
		返回当前时间字符串
	*/
	static std::string GetLocalTimeStr();

	/*!
		返回当前日期时间的字符串
	*/
	static std::string GetLocalDateTimeStr(char datesep = '-', char timesep = ':');

	/*!
		返回时间字符串
	*/
	static std::string FormatTimeStr(int intime, char separator = ':');

	/*!
		返回日期字符串
	*/
	static std::string FormatDateStr(int indate, char separator = '-');
};

}// namespace lang

#endif //_LANG_DATEUTIL_H
