#include "lang/DateUtil.h"

namespace lang {

bool TDateUtil::ValidDate(int indate)
{
	static int day_tbl[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    static int leapday_tbl[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year = indate / 10000;
    int month = (indate / 100) % 100;
    int day = indate % 100;

	if (year < 1900 || year > 5999 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;
    if (IsLeapYear(year))
        return (day > leapday_tbl[month] ? false : true);
    else
	    return (day > day_tbl[month] ? false : true);
}

bool TDateUtil::ValidTime(int intime)
{
	int   hh,mm,ss;
	hh = intime / 10000;
	mm = (intime / 100) % 100;
	ss = intime % 100;
	return !(hh < 0 || hh > 24 || mm < 0 || mm > 60 || ss < 0 || ss > 60);
}

bool TDateUtil::IsLeapYear(int year)
{
	return (((year%4) == 0 && (year%100) !=0 ) || (year%400) == 0);
}

int TDateUtil::MaxDay(int year, int month)
{
	int	 max_day;
	if (month == 2)
		max_day = 28 + (IsLeapYear(year) ? 1 : 0);
	else
	{       
		if(month >= 8)
			month ++;
		max_day = 30 + (month % 2);
	}
	return(max_day);
}

int TDateUtil::NetDayFrom1970(int indate)
{
	int i, datecount = 0;
    int year = indate / 10000;
    int month = (indate / 100) % 100;
    int day = indate % 100;

    if (year<1970 || month<1 || month>12 || day<1 || day>31)
        return -1;
    for(i = 1970; i < year; i++)
    {
        datecount += 365;
        if (IsLeapYear(i))
            datecount ++;
    }

    switch (month)
    {
        case 1: break;
        case 2: datecount += 31; break;
        case 3: datecount += 31+28; break;
        case 4: datecount += 31+28+31; break;
        case 5: datecount += 31+28+31+30; break;
        case 6: datecount += 31+28+31+30+31; break;
        case 7: datecount += 31+28+31+30+31+30; break;
        case 8: datecount += 31+28+31+30+31+30+31; break;
        case 9: datecount += 31+28+31+30+31+30+31+31; break;
        case 10: datecount +=31+28+31+30+31+30+31+31+30; break;
        case 11: datecount +=31+28+31+30+31+30+31+31+30+31; break;
        case 12: datecount +=31+28+31+30+31+30+31+31+30+31+30; break;
    }

    if(month > 2)
    {
        if (IsLeapYear(year))
            datecount ++;
    }
    datecount += day - 1;
    return datecount;
}

int TDateUtil::BetweenDays(int startdate, int enddate)
{
    int startdays = NetDayFrom1970(startdate);
    if (startdays == -1)
        return 0;
    int enddays = NetDayFrom1970(enddate);
    if ( enddays == -1 )
        return 0;
	return enddays - startdays;
}

int TDateUtil::BetweenTimes(int first,int second)
{
    int span = second/10000*3600+second%10000/100*60+second%100
        - (first/10000*3600+first%10000/100*60+first%100);
    return span;
}

int TDateUtil::AddDay(int indate, int day_offset)
{
    struct tm nowtm;
    struct tm *c_tm;
    char szDateTime[9];
    memset(&nowtm, 0, sizeof(struct tm));
    memset(szDateTime, 0, 9);

    nowtm.tm_year = indate / 10000 - 1900;
    nowtm.tm_mon = (indate / 100) % 100 - 1;
    nowtm.tm_mday = indate % 100;
    time_t t1 = mktime(&nowtm);
    t1 += day_offset * 86400L;
    c_tm = localtime(&t1);
    sprintf(szDateTime,"%4.4d%2.2d%2.2d", 1900+c_tm->tm_year, c_tm->tm_mon+1, c_tm->tm_mday);
	return atoi(szDateTime);
}

int TDateUtil::GetLocalDate()
{
	struct tm *c_tm;
	time_t t1;
    char szDateTime[9];
    memset(szDateTime, 0, 9);
	t1 = time(NULL);
	c_tm = localtime(&t1);
	sprintf(szDateTime,"%4.4d%2.2d%2.2d", 1900+c_tm->tm_year, c_tm->tm_mon+1, c_tm->tm_mday);
	return atoi(szDateTime);
}

int TDateUtil::GetLocalTime()
{
	struct tm *c_tm;
	time_t t1;
	char szDateTime[7];
    memset(szDateTime, 0, 7);
	t1 = time(NULL);
	c_tm = localtime(&t1);
	sprintf(szDateTime, "%2.2d%2.2d%2.2d", c_tm->tm_hour, c_tm->tm_min, c_tm->tm_sec);
	return atoi(szDateTime);
}

std::string TDateUtil::GetLocalTimeStr()
{
	struct tm *c_tm;
	time_t t1;
	char szDateTime[10];
    memset(szDateTime, 0, 10);
	t1 = time(NULL);
	c_tm = localtime(&t1);
	sprintf(szDateTime, "%2.2d:%2.2d:%2.2d", c_tm->tm_hour, c_tm->tm_min, c_tm->tm_sec);
	return std::string(szDateTime);
}

std::string TDateUtil::GetLocalDateTimeStr(char datesep, char timesep)
{
	struct tm *c_tm;
	time_t t1;
	char szDateTime[25];
    memset(szDateTime, 0, 25);
	t1 = time(NULL);
	c_tm = localtime(&t1);
	sprintf(szDateTime, "%4.4d%c%2.2d%c%2.2d %2.2d%c%2.2d%c%2.2d", 
		1900+c_tm->tm_year, datesep, c_tm->tm_mon+1, datesep, c_tm->tm_mday, 
		c_tm->tm_hour, timesep, c_tm->tm_min, timesep, c_tm->tm_sec);
	return std::string(szDateTime);
}

std::string TDateUtil::FormatTimeStr(int intime, char separator)
{
    int   hh,mm,ss;
	hh = intime / 10000;
	mm = (intime / 100) % 100;
	ss = intime % 100;
    char szDateTime[12];
    memset(szDateTime, 0, 12);
    sprintf(szDateTime,"%2.2d%c%2.2d%c%2.2d", hh, separator, mm, separator, ss);
	return std::string(szDateTime);
}

std::string TDateUtil::FormatDateStr(int indate, char separator)
{
    int year = indate / 10000;
    int month = (indate / 100) % 100;
    int day = indate % 100;
    char szDateTime[12];
    memset(szDateTime, 0, 12);
    sprintf(szDateTime,"%4.4d%c%2.2d%c%2.2d", year, separator, month, separator, day);
	return std::string(szDateTime);
}

}
