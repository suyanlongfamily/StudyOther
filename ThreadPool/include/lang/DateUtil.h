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
		�ж��Ƿ�����Ч���� yyyymmdd
	*/
	static bool ValidDate(int indate);

	/*!
		�ж��Ƿ�����Чʱ�� hhmmss
	*/
	static bool ValidTime(int intime);

	/*!
		�ж��Ƿ�������
	*/
	static bool IsLeapYear(int year);

	/*!
		����һ�����е��������
	*/
	static int MaxDay(int year, int month);

	/*!
		���ش�1970��ʼ�ľ�����
	*/
	static int NetDayFrom1970(int indate);

	/*!
		������������֮����������
	*/
	static int BetweenDays(int startdate, int enddate);

    /*!
		��������ʱ��֮����������
	*/
    static int BetweenTimes(int first,int second);

	/*!
		��������
	*/
	static int AddDay(int indate, int day_offset);

	/*!
		���ص�ǰ����
	*/
	static int GetLocalDate();

	/*!
		���ص�ǰʱ��
	*/
	static int GetLocalTime();

	/*!
		���ص�ǰʱ���ַ���
	*/
	static std::string GetLocalTimeStr();

	/*!
		���ص�ǰ����ʱ����ַ���
	*/
	static std::string GetLocalDateTimeStr(char datesep = '-', char timesep = ':');

	/*!
		����ʱ���ַ���
	*/
	static std::string FormatTimeStr(int intime, char separator = ':');

	/*!
		���������ַ���
	*/
	static std::string FormatDateStr(int indate, char separator = '-');
};

}// namespace lang

#endif //_LANG_DATEUTIL_H
