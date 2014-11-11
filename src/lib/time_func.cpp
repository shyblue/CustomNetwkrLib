#include <chrono>
#include <string>

#include "common.h"
#include "util/time_func.h"

TimeFunc::TimeFunc(void)
{
}

TimeFunc::~TimeFunc(void)
{
}

void TimeFunc::GetCurrentTimeToString_nospecialchar(std::string& strtime)
{
	const size_t kdatelength = 64;
	char date[kdatelength];
	GetCurrentTimeToString(date, kdatelength);
	strtime.append(date);
	strtime.erase('-');
	strtime.erase(':');
	strtime.erase('[');
	strtime.erase(']');
}

void TimeFunc::GetCurrentTimeToString(std::string& strtime)
{
	const size_t kdatelength = 64;
	char date[kdatelength];
	GetCurrentTimeToString(date, kdatelength);
	strtime.append(date);
}

void TimeFunc::GetCurrentTimeToString(char * strtime, size_t size)
{
#ifdef __linux
	timeval tv;
	gettimeofday(&tv, NULL);

	struct tm* tm = localtime((time_t*)&tv.tv_sec);

	SNPRINTF(strtime, size, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ", 
		tm->tm_year + 1900,
		tm->tm_mon + 1,
		tm->tm_mday,
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec,
		(int32_t)(tv.tv_usec/1000)
		);
#else
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);

	SNPRINTF(strtime, size, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ", 
		systemtime.wYear,
		systemtime.wMonth,
		systemtime.wDay,
		systemtime.wHour,
		systemtime.wMinute,
		systemtime.wSecond,
		systemtime.wMilliseconds
		);
#endif
}