#pragma  once

#ifndef _UTILITY_TIMEFUNC_H_
#define _UTILITY_TIMEFUNC_H_

class TimeFunc
{
public:
	TimeFunc(void);
	~TimeFunc(void);

	static void GetCurrentTimeToString(std::string& strtime);
	static void GetCurrentTimeToString(char * strtime, size_t size);

	static void GetCurrentTimeToString_nospecialchar(std::string& strtime);
private:
};


#endif //_UTILITY_TIMEFUNC_H_